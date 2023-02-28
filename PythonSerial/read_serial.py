import serial.tools.list_ports
import time
import datetime
from datetime import datetime

ports = serial.tools.list_ports.comports()
serialInst = serial.Serial()
portsList = []

beginMarker = '<'
endMarker = '>'
keyChars = "<>"
message = ''

#nomeArquivo = datetime.now().strftime("%Y%m%d-%H%M%S") + ".txt"
nomeArquivo = "hidrosens.txt"
for onePort in ports:
    portsList.append(str(onePort))
    print(str(onePort))

val = input("Select Port: COM")

for x in range(0,len(portsList)):
    if portsList[x].startswith("COM" + str(val)):
        portVar = "COM" + str(val)
        print(portVar)

serialInst.baudrate = 9600
serialInst.port = portVar
serialInst.open()

while True:
    arquivo = open(nomeArquivo, "a+")
    time.sleep(0.01)
    if serialInst.in_waiting:
        message = serialInst.readline().decode('utf').rstrip('\t\n')
        #print(packet.decode('utf').rstrip('\n'))
        if (beginMarker in message and endMarker in message):        
            for character in keyChars:
                message = message.replace(character, "")
            print(message)
            arquivo.write(message)
            serialInst.reset_input_buffer()
