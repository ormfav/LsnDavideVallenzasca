#ifndef __DataBlocking__
#define __DataBlocking__

#include "../Misc/include/misc.h"
#include "../Point/include/point.h"
#include "../Random/include/random.h"
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>

using namespace std;
/* Can be read as: datablocking for `PARAMS` evolving, according to
 * `move` function, parameters in order to evaluate `PROPS`
 * properties; each `eval_prop` function evaluate a property using
 * parameters value */
template <size_t PARAMS, size_t PROPS> class dataBlocks {
public:
  dataBlocks(size_t steps, function<bool(point<double, PARAMS> &)> move,
             array<function<double(point<double, PARAMS>)>, PROPS> eval_props);
  dataBlocks(size_t steps, array<double, PARAMS> x0,
             function<bool(point<double, PARAMS> &)> move,
             array<function<double(point<double, PARAMS>)>, PROPS> eval_props);

  /* Measure properties in a single block */
  double Measure();
  double Measure(int, ofstream &);

  /* Transform a property evaluated in a block and its uncertainty */
  void Map(size_t prop, function<double(double)> map_val,
           function<double(double, double)> map_err);

  /* Evaluate cumulative averages for the current block */
  void EvalBlock(array<ofstream, PROPS> &);
  void EvalBlock();

  array<double, PROPS> GetBlkAve();
  array<double, PROPS> GetBlkErr();
  array<double, PROPS> GetPrgAve();
  array<double, PROPS> GetPrgErr();

private:
  const size_t STEPS_; // steps per block
  point<double, PARAMS> params_;

  array<function<double(point<double, PARAMS>)>, PROPS> eval_props_;

  // value and error of the single block
  array<double, PROPS> blk_ave_;
  array<double, PROPS> blk_err_;
  // progressive average and error of the blocks
  array<double, PROPS> prg_ave_;
  array<double, PROPS> prg_av2_; // aux: needed to evaluate progressive error
  array<double, PROPS> prg_err_;

  size_t nblk_;
};

template <size_t PARAMS, size_t PROPS>
dataBlocks<PARAMS, PROPS>::dataBlocks(
    size_t steps, function<bool(point<double, PARAMS> &)> move,
    array<function<double(point<double, PARAMS>)>, PROPS> eval_props)
    : STEPS_(steps), params_(move) {
  eval_props_ = eval_props;
  blk_ave_ = {0};
  blk_err_ = {0};
  prg_ave_ = {0};
  prg_av2_ = {0};
  prg_err_ = {0};
  nblk_ = 0;
}

template <size_t PARAMS, size_t PROPS>
dataBlocks<PARAMS, PROPS>::dataBlocks(
    size_t steps, array<double, PARAMS> x0,
    function<bool(point<double, PARAMS> &)> move,
    array<function<double(point<double, PARAMS>)>, PROPS> eval_props)
    : STEPS_(steps), params_(x0, move) {
  eval_props_ = eval_props;
  blk_ave_ = {0};
  blk_err_ = {0};
  prg_ave_ = {0};
  prg_av2_ = {0};
  prg_err_ = {0};
  nblk_ = 0;
}

template <size_t PARAMS, size_t PROPS>
double dataBlocks<PARAMS, PROPS>::Measure() {
  array<double, PROPS> av2 = {0};
  for (double &x : blk_ave_)
    x = 0;

  for (size_t i = 0; i < STEPS_; ++i) {
    params_.Move();
    for (size_t j = 0; j < PROPS; ++j) {
      double aux = eval_props_[j](params_);
      blk_ave_[j] += aux;
      av2[j] += aux * aux;
    }
  }

  for (size_t i = 0; i < PROPS; ++i)
    blk_err_[i] = Error(blk_ave_[i], av2[i], STEPS_);
  for (double &x : blk_ave_)
    x /= STEPS_;
  return params_.Rate();
}

template <size_t PARAMS, size_t PROPS>
double dataBlocks<PARAMS, PROPS>::Measure(size_t interval, ofstream &fout) {
  array<double, PROPS> av2 = {0};
  for (double &x : blk_ave_)
    x = 0;

  for (size_t i = 0; i < STEPS_; ++i) {
    params_.Move();
    if ((nblk_ * STEPS_ + i) % interval == 0) {
      string to_prsize_t = "";
      for (double x : params_.ToArray()) {
        to_print.append(to_string(x));
        to_print.push_back(',');
      }
      to_print.pop_back(); // delete last comma
      fout << to_prsize_t << endl;
    }

    for (size_t j = 0; j < PROPS; ++j) {
      double aux = eval_props_[j](params_);
      blk_ave_[j] += aux;
      av2[j] += aux * aux;
    }
  }

  for (size_t i = 0; i < PROPS; ++i)
    blk_err_[i] = Error(blk_ave_[i], av2[i], STEPS_);
  for (double &x : blk_ave_)
    x /= STEPS_;
  return params_.Rate();
}

template <size_t PARAMS, size_t PROPS>
void dataBlocks<PARAMS, PROPS>::EvalBlock(array<ofstream, PROPS> &fout) {
  if (!nblk_)
    for (ofstream &x : fout)
      x << "block_number,block_average,block_error,progressive_average,"
           "progressive_error\n";

  nblk_++;
  for (size_t i = 0; i < PROPS; ++i) {
    prg_ave_[i] += blk_ave_[i];
    prg_av2_[i] += blk_ave_[i] * blk_ave_[i];
    prg_err_[i] = Error(prg_ave_[i], prg_av2_[i], nblk_);
    fout[i] << nblk_ << "," << blk_ave_[i] << "," << blk_err_[i] << ","
            << prg_ave_[i] / nblk_ << "," << prg_err_[i] << endl;
    /* cout << nblk_ << "," << blk_ave_[i] << "," << blk_err_[i] << "," <<
     * prg_ave_[i]/nblk_ << "," << prg_err_[i] << endl; */
  }
}

template <size_t PARAMS, size_t PROPS>
void dataBlocks<PARAMS, PROPS>::EvalBlock() {
  nblk_++;
  for (size_t i = 0; i < PROPS; ++i) {
    prg_ave_[i] += blk_ave_[i];
    prg_av2_[i] += blk_ave_[i] * blk_ave_[i];
    prg_err_[i] = Error(prg_ave_[i], prg_av2_[i], nblk_);
  }
}

template <size_t PARAMS, size_t PROPS>
void dataBlocks<PARAMS, PROPS>::Map(size_t prop,
                                    function<double(double)> map_val,
                                    function<double(double, double)> map_err) {
  if (prop >= PROPS) {
    cout << "ERROR: out of range property index\n";
    exit(1);
  }
  blk_ave_[prop] = map_val(blk_ave_[prop]);
  blk_err_[prop] = map_err(blk_ave_[prop], blk_err_[prop]);
}

template <size_t PARAMS, size_t PROPS>
array<double, PROPS> dataBlocks<PARAMS, PROPS>::GetBlkAve() {
  return blk_ave_;
}

template <size_t PARAMS, size_t PROPS>
array<double, PROPS> dataBlocks<PARAMS, PROPS>::GetBlkErr() {
  return blk_err_;
}

template <size_t PARAMS, size_t PROPS>
array<double, PROPS> dataBlocks<PARAMS, PROPS>::GetPrgAve() {
  array<double, PROPS> out = prg_ave_;
  for (double &x : out)
    x /= nblk_;
  return out;
}

template <size_t PARAMS, size_t PROPS>
array<double, PROPS> dataBlocks<PARAMS, PROPS>::GetPrgErr() {
  return prg_err_;
}

#endif
