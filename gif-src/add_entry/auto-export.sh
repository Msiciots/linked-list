for ip in $(seq 1 6)
do
   # echo ${ip}
    dot -Tpng ${ip}.dot > output${ip}.png 
done
