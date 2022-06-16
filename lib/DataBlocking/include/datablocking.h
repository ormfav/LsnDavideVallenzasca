#ifndef __DataBlocking__
#define __DataBlocking__

#include "../../Misc/include/misc.h"
#include "../../Point/include/point.h"
#include "../../Random/include/random.h"
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
             array<function<double(point<double, PARAMS>)>, PROPS> eval_props,
             array<double, PARAMS> x0 = {0});

  /* Evolve parameters and measure properties in a single block.
   * Returns parameters evolution acceptance rates */
  double Measure();
  /* Measure and print parameters every k steps (from the beginning of
   * the datablocking procedure) */
  double Measure(size_t k, size_t nblk, ofstream &);

  /* Transform a property evaluated in a block */
  void Map(size_t prop, function<double(double)> map_val);

  /* Evaluate (and print) cumulative averages for the current block */
  void EvalBlock(array<ofstream, PROPS> &);
  void EvalBlock();

  array<double, PROPS> GetBlkAve();
  array<double, PROPS> GetPrgAve();
  array<double, PROPS> GetPrgErr();

  size_t CompletedBlocks() { return nblk_; }
  void Restart();

private:
  const size_t STEPS_; // steps per block
  point<double, PARAMS> params_;

  array<function<double(point<double, PARAMS>)>, PROPS> eval_props_;

  // average of the single block
  array<double, PROPS> blk_ave_;
  // progressive average and error of the blocks
  array<double, PROPS> prg_ave_;
  array<double, PROPS> prg_av2_; // aux: needed to evaluate progressive error
  array<double, PROPS> prg_err_;

  size_t nblk_;
};

template <size_t PARAMS, size_t PROPS>
dataBlocks<PARAMS, PROPS>::dataBlocks(
    size_t steps, function<bool(point<double, PARAMS> &)> move,
    array<function<double(point<double, PARAMS>)>, PROPS> eval_props,
    array<double, PARAMS> x0)
    : STEPS_(steps), params_(move, x0) {
  eval_props_ = eval_props;
  blk_ave_ = {0};
  prg_ave_ = {0};
  prg_av2_ = {0};
  prg_err_ = {0};
  nblk_ = 0;
}

template <size_t PARAMS, size_t PROPS>
double dataBlocks<PARAMS, PROPS>::Measure() {
  for (double &x : blk_ave_)
    x = 0;

  for (size_t i = 0; i < STEPS_; ++i) {
    params_.Move();
    for (size_t j = 0; j < PROPS; ++j)
      blk_ave_[j] += eval_props_[j](params_);
  }

  for (double &x : blk_ave_)
    x /= STEPS_;
  return params_.Rate();
}

template <size_t PARAMS, size_t PROPS>
double dataBlocks<PARAMS, PROPS>::Measure(size_t k, size_t nblk,
                                          ofstream &fout) {
  for (double &x : blk_ave_)
    x = 0;

  for (size_t i = 0; i < STEPS_; ++i) {
    params_.Move();
    if ((nblk * STEPS_ + i) % k == 0) {
      string to_print = "";
      for (double x : params_.ToArray()) {
        to_print.append(to_string(x));
        to_print.push_back(',');
      }
      to_print.pop_back(); // delete last comma
      fout << to_print << endl;
    }
    for (size_t j = 0; j < PROPS; ++j)
      blk_ave_[j] += eval_props_[j](params_);
  }

  for (double &x : blk_ave_)
    x /= STEPS_;
  return params_.Rate();
}

template <size_t PARAMS, size_t PROPS>
void dataBlocks<PARAMS, PROPS>::EvalBlock(array<ofstream, PROPS> &fout) {
  if (!nblk_) // if first block
    for (ofstream &x : fout)
      x << "block_number,block_average,block_error,progressive_average,"
           "progressive_error\n";

  nblk_++;
  for (size_t i = 0; i < PROPS; ++i) {
    prg_ave_[i] += blk_ave_[i];
    prg_av2_[i] += blk_ave_[i] * blk_ave_[i];
    prg_err_[i] = Error(prg_ave_[i], prg_av2_[i], nblk_);
    fout[i] << nblk_ << "," << blk_ave_[i] << "," << prg_ave_[i] / nblk_ << ","
            << prg_err_[i] << endl;
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
                                    function<double(double)> map_val) {
  if (prop >= PROPS) {
    cout << "ERROR: out of range property index\n";
    exit(1);
  }
  blk_ave_[prop] = map_val(blk_ave_[prop]);
}

template <size_t PARAMS, size_t PROPS>
array<double, PROPS> dataBlocks<PARAMS, PROPS>::GetBlkAve() {
  return blk_ave_;
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

template <size_t PARAMS, size_t PROPS>
void dataBlocks<PARAMS, PROPS>::Restart() {
  nblk_ = 0;
  prg_ave_ = {0};
  prg_av2_ = {0};
  prg_err_ = {0};
}
#endif
