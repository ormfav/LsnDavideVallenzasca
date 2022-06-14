BEGIN{FS=" "}
{gsub(/[a-zA-Z]/,""); $1=$1}
(NF>2){$1=""; sub(/^[ \t]+/, "")}
(NR==1){print "longitude latitude"}
(NR!=1){print}
END{}
