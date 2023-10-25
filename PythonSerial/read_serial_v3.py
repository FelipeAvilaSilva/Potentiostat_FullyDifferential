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

def writetxt():
    with open(nomeArquivo, "a+") as arquivo:
        while True:
            if serialInst.in_waiting:
                message = serialInst.readline().decode('utf').rstrip('\t\n')

                if FlagChars in message:
                    break  # Saia do loop quando encontrar FlagChars

                if beginMarker2 in message and endMarker2 in message:
                    padrao = r'\{(.*?)\}'
                    match = re.search(padrao, message)
                    if match:
                        strings = match.group(1).split(';')
                        strings = [s.strip() for s in strings if s.strip()]
                        tmp0, tmp1, tmp2 = strings  # Atribuição múltipla para descompactar os valores

                if beginMarker in message and endMarker in message:
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
    ax.set_title(f'Gráfico de Voltametria Cíclica\n\nScan Rate: {scanRate} mV/s | Start Potential: {startPotential} V | End Potential: {endPotential} V | Loop: {loop}', fontsize=10)
    plt.figtext(0.99, 0.01, f'Gerado em {dt.datetime.now().strftime("%d/%m/%Y %H:%M:%S")}', horizontalalignment='right')
    plt.show()



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
nomeArquivo = nomeArquivo = input("Digite o nome do arquivo a ser salvo (ex: meu_arquivo): ") + ".txt"

for onePort in ports:
    portsList.append(str(onePort))
    print(str(onePort))

#Descomentar a constante em outros PC's
#input("Digite o número da porta do seu Arduino")
    
val = 5
for x in range(0,len(portsList)):
    if portsList[x].startswith("COM" + str(val)):
        portVar = "COM" + str(val)
        print(portVar)

serialInst.baudrate = 19200
serialInst.port = portVar
serialInst.open()

while True:
    print("\nSelecione uma técnica:")
    print("1 - Voltametria Cíclica")
    print("2 - Voltametria de Varredura Linear")
    print("3 - Cronoamperometria")

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
        selection = input("\nInclua SCAN RATE \nFaixa Permitida: 1 - 250 mV/s\n")
        serialInst.write(str(selection).encode())
        print("Scan rate = " + selection + " mV/s")
        scanRate = selection
        time.sleep(2)

        selection = input("\nInclua Start potential \nFaixa Permitida: -Vmin to +Vmax volts\n")
        serialInst.write(str(selection).encode())
        print("Start Potential = " + str(selection) + " V")
        startPotential = selection
        time.sleep(2)

        selection = input("\nInclua End potential \nFaixa Permitida: -Vmin to +Vmax volts\n")
        serialInst.write(str(selection).encode())
        print("End Potential = " + selection + " V")
        endPotential = selection
        time.sleep(2)

        selection = input("\nInclua Loop \nFaixa Permitida: Loop >= 1 \n")
        serialInst.write(str(selection).encode())
        print("Loop = " + selection + "x\n")
        loop = selection
        time.sleep(2)

        print("Aguarde")
        writetxt()
        writegraph(scanRate, startPotential, endPotential)

