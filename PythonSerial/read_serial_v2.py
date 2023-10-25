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

def  writetxt():
    loop = True
    while loop == True:        
        arquivo = open(nomeArquivo, "a+")
        time.sleep(0.1)      
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
    plt.figtext(0.99, 0.01, f'Gerado em {dt.datetime.now().strftime("%d/%m/%Y %H:%M:%S")}', horizontalalignment='right')
    plt.show()

    # Restante do código
    # ...




    df = pd.read_csv(nomeArquivo, delimiter=';', skipinitialspace=True)
    df['potencial'] = df.iloc[:, 0]
    df['potencial'] = df['potencial'][::-1].reset_index(drop=True)
    sentidos = ['Decida' if df['potencial'][i] > df['potencial'][i+1] else 'Subida' for i in range(len(df)-1)]
    sentidos.insert(0, 'Decida')

    title_text = 'Gráfico de Voltametria Cíclica<br>Scan Rate: {0} mV/s |   Start Potential: {1} V  |   End Potential: {2} V'.format(scanRate, startPotential, endPotential)

    fig = px.line(df, x='potencial', y=df.columns[1], color=sentidos, width=800, height=600,
                  title=title_text)
    fig.update_layout(
        title={
            'text': title_text,
            'y': 0.98,
            'x': 0.5,
            'xanchor': 'center',
            'yanchor': 'top',
            'font': {'size': 16}
        },
        xaxis_title='Potencial (V)',
        yaxis_title='Corrente (uA)',
        hovermode='closest',
        dragmode='pan',
        margin=dict(l=50, r=50, b=50, t=50, pad=4),
        uirevision='constant'
    )
    fig.add_annotation(text=f'Gerado em {dt.datetime.now().strftime("%d/%m/%Y %H:%M:%S")}', xref='paper', yref='paper',
                       x=1, y=0, showarrow=False, font={'size': 12})
    fig.write_html(temp + ".html")




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

serialInst.baudrate = 19200
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
        time.sleep(1)

        selection = input("\nEnter Start potential \nALLOWED RANGE: -1.36 to +1.41 volts\n")
        serialInst.write(str(selection).encode())
        print("Start Potential: " + str(selection) + " V")
        startPotential = selection
        time.sleep(1)

        selection = input("\nEnter End potential \nALLOWED RANGE: -1.36 to +1.41 volts\n")
        serialInst.write(str(selection).encode())
        print("End Potential: " + selection + " V\n")
        endPotential = selection
        time.sleep(1)

        print("Aguarde")
        writetxt()
        writegraph(scanRate, startPotential, endPotential)
