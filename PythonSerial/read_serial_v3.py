import serial.tools.list_ports
import time
import datetime
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import plotly.express as px
import datetime as dt
from datetime import datetime
import re
from io import BytesIO
import base64

def writetxt():
    loop = True
    while loop == True:        
        arquivo = open(nomeArquivo, "a+")
        time.sleep(0.05)      
        if serialInst.in_waiting:
            message = serialInst.readline().decode('utf').rstrip('\t\n')            

            if (FlagChars in message):
                loop = False

            if (beginMarker2 in message and endMarker2 in message):
                padrao = r'\{(.*?)\}'

                match = re.search(padrao, message)
                if match:
                    strings = match.group(1).split(';')
                    strings = [s.strip() for s in strings if s.strip()]
                    tmp0 = strings[0]
                    tmp1 = strings[1]
                    tmp2 = strings[2]

            if (beginMarker in message and endMarker in message):              
                for character in keyChars:
                    message = message.replace(character, "")                              
                for character in FlagChars:                    
                    message = message.replace(character, "")                
                print(message)
                arquivo.write(message.strip(" "))
                serialInst.reset_input_buffer()

def writegraph(scanRate, startPotential, endPotential):
    df = pd.read_csv(nomeArquivo, delimiter=';', skipinitialspace=True)
    df['potencial'] = df.iloc[:, 0]
    df['potencial'] = df['potencial'][::-1].reset_index(drop=True)

    cores = ['blue' if df['potencial'][i] < df['potencial'][i+1] else 'red' for i in range(len(df)-1)]
    cores.insert(0, 'blue')

    fig, ax = plt.subplots()
    for i in range(len(df)-1):
        ax.plot([df['potencial'][i], df['potencial'][i+1]], [df.iloc[i, 1], df.iloc[i+1, 1]], color=cores[i], linewidth=1, solid_capstyle='round')

    ax.set_xlabel('Potencial (V)')
    ax.set_ylabel('Corrente (uA)')
    ax.set_title(f'Gráfico de Voltametria Cíclica\n\nScan Rate: {scanRate} mV/s | Start Potential: {startPotential} V | End Potential: {endPotential} V', fontsize=10)
    ax.text(0.99, 0.01, f'Gerado em {dt.datetime.now().strftime("%d/%m/%Y %H:%M:%S")}', horizontalalignment='right', transform=ax.transAxes)

    # Salva o gráfico em memória
    buffer = BytesIO()
    plt.savefig(buffer, format='png')
    buffer.seek(0)

    # Converte o gráfico em base64
    image_base64 = base64.b64encode(buffer.read()).decode('utf-8')

    # Cria o elemento HTML para exibir a imagem
    image_html = f'<img src="data:image/png;base64,{image_base64}">'

    plt.close()  # Fecha a figura para liberar memória

    # Exibe o HTML do gráfico
    display.display(display.HTML(image_html))



ports = serial.tools.list_ports.comports()
serialInst = serial.Serial()
portsList = []

beginMarker = '<'
endMarker = '>'

beginMarker2 = '{'
endMarker2 = '}'

keyChars = "<>"
FlagChars = '<finish>'
message = ''

temp = datetime.now().strftime("%Y%m%d-%H%M%S")
nomeArquivo = temp + ".txt"

for onePort in ports:
    portsList.append(str(onePort))
    print(str(onePort))

val = 5
for x in range(0,len(portsList)):
    if portsList[x].startswith("COM" + str(val)):
        portVar = "COM" + str(val)
        print(portVar)

serialInst.baudrate = 9600
serialInst.port = portVar
serialInst.open()

while True:
    print("\nSelecione uma técnica:")
    print("1 - CYCLIC VOLTAMMETRY")
    print("2 - LINEAR SWEEP VOLTAMMETRY")
    print("3 - CHRONOAMPEROMETRY")

    while True:
        try:
            selection = int(input("Digite sua escolha: "))
            if selection in [1, 2, 3]:
                break
            else:
                print("Opção inválida! Tente novamente.")
        except ValueError:
            print("Entrada inválida! Tente novamente.")

    serialInst.write(str(selection).encode())

    if selection == 1:
        selection = input("\nENTER SCAN RATE \nALLOWED RANGE: 1 - 250 mV/s\n")
        serialInst.write(str(selection).encode())
        print("Scan rate: " + selection + " mV/s")
        scanRate = selection

        selection = input("\nEnter Start potential \nALLOWED RANGE: -1.36 to +1.41 volts\n")
        serialInst.write(str(selection).encode())
        print("Start Potential: " + str(selection) + " V")
        startPotential = selection

        selection = input("\nEnter End potential \nALLOWED RANGE: -1.36 to +1.41 volts\n")
        serialInst.write(str(selection).encode())
        print("End Potential: " + selection + " V\n")
        endPotential = selection

        print("Aguarde")
        writetxt()
        writegraph(scanRate, startPotential, endPotential)
