test_num_clients ()
{
    for i in `seq 1 $1` ;
    do
        #5 seconds
        if [ $((i%3)) -eq 0 ]
        then
            ./cgps 10 0 > client_log/test_r$1_$i_0.txt &
        fi

        #10 seconds
        if [ $((i%3)) -eq 1 ] 
        then
            ./cgps 10 50 > client_log/test_r$1_$i_50.txt &
        fi
        #15 seconds
        if [ $((i%3)) -eq 2 ]
        then
            ./cgps 10 99 > client_log/test_r$1_$i_99.txt &
        fi
    done
}

test_num_clients $1 
