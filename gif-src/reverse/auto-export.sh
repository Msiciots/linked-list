for ip in $(seq 1 10)
do
   # echo ${ip}
    dot -Tpng ${ip}.dot > output${ip}.png 
done
