import time
import subprocess
n_clients = 10




#filename = "client_log/client_" + str(1) + ".log"
#subprocess.run(["touch", filename])
#subprocess.run(["./cgps" ,">", filename])


for i in range(0,n_clients):
    time.sleep(5)
    
    filename = "client_log/client_" + str(i) + ".log"
    subprocess.run(["touch", filename])
    subprocess.run(["./cgps" ,">", filename])
    
