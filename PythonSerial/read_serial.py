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
    scanRate = 0
    startPotential = 0
    EndPotential = 0
    loop = True
    while loop == True:        
        arquivo = open(nomeArquivo, "a+")
        time.sleep(0.01)      
        if serialInst.in_waiting:
            message = serialInst.readline().decode('utf').rstrip('\t\n')            
            #print(packet.decode('utf').rstrip('\n'))            
            if (FlagChars in message):
                    loop = False

            if (beginMarker2 in message and endMarker2 in message):
                #message = "{string1, string2, string3}"  
                padrao = r'\{(.*?)\}'

                match = re.search(padrao, message)
                if match:
                    strings = match.group(1).split(';')
                    strings = [s.strip() for s in strings if s.strip()]
                    #print(strings)
                    tmp0 = strings[0]
                    tmp1 = strings[1]
                    tmp2 = strings[2]
                    #print(tmp0)
                

                
            if (beginMarker in message and endMarker in message):              
                for character in keyChars:
                    message = message.replace(character, "")                              
                for character in FlagChars:                    
                    message = message.replace(character, "")                
                print(message)
                arquivo.write(message.strip(" "))
                serialInst.reset_input_buffer()

def writegraph():
    # Leitura do arquivo "hidrosens.txt" usando a biblioteca Pandas
    df = pd.read_csv(nomeArquivo, delimiter=';', skipinitialspace=True)

    # Criação de uma nova coluna de potencial para o gráfico
    df['potencial'] = df.iloc[:, 0]

    # Inverter a ordem dos valores para que a tensão seja crescente
    df['potencial'] = df['potencial'][::-1].reset_index(drop=True)

    # Determina a mudança de cor com base na direção da varredura
    cores = np.where(np.diff(df['potencial']) > 0, 'red', 'blue')

    #começa pelo vermelho
    cores = np.append('red', cores)

    # Criação do gráfico de voltametria cíclica usando a biblioteca Matplotlib
    fig, ax = plt.subplots()
    ax.scatter(df['potencial'], df.iloc[:, 1], c=cores, s=0.3)
    ax.set_xlabel('Potencial (V)')
    ax.set_ylabel('Corrente (uA)')
    ax.set_title('Gráfico de Voltametria Cíclica')
    plt.figtext(0.99, 0.01, f'Gerado em {dt.datetime.now().strftime("%d/%m/%Y %H:%M:%S")}', horizontalalignment='right')
    plt.show()

    # Leitura do arquivo "hidrosens.txt" usando a biblioteca Pandas
    df = pd.read_csv(nomeArquivo, delimiter=';', skipinitialspace=True)

    # Criação de uma nova coluna de potencial para o gráfico
    df['potencial'] = df.iloc[:, 0]

    # Inverter a ordem dos valores para que a tensão seja crescente
    df['potencial'] = df['potencial'][::-1].reset_index(drop=True)

    # Determina a mudança de cor com base na direção da varredura
    cores = ['red' if df['potencial'][i] > df['potencial'][i+1] else 'blue' for i in range(len(df)-1)]
    cores.insert(0, 'red')

    # Criação do gráfico de voltametria cíclica usando a biblioteca Plotly
    fig = px.scatter(df, x='potencial', y=df.columns[1], color=cores, width=800, height=600,
                 title='Gráfico de Voltametria Cíclica')
    fig.update_layout(
    title={
        'y':0.98,
        'x':0.5,
        'xanchor': 'center',
        'yanchor': 'top'},
    xaxis_title='Potencial (V)',
    yaxis_title='Corrente (uA)',
    font=dict(size=16),
    hovermode='closest',
    dragmode='pan',
    margin=dict(l=50, r=50, b=50, t=50, pad=4),
    uirevision='constant'
    )

    # Adiciona a data e hora de criação da figura
    fig.add_annotation(text=f'Gerado em {dt.datetime.now().strftime("%d/%m/%Y %H:%M:%S")}', xref='paper', yref='paper',
                   x=1, y=0, showarrow=False, font=dict(size=12))

    # Salva a figura interativa como um arquivo HTML
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
#nomeArquivo = "hidrosens.txt"
for onePort in ports:
    portsList.append(str(onePort))
    print(str(onePort))

#val = input("Select Port: COM")
val = 5
for x in range(0,len(portsList)):
    if portsList[x].startswith("COM" + str(val)):
        portVar = "COM" + str(val)
        print(portVar)

serialInst.baudrate = 9600
serialInst.port = portVar
serialInst.open()

#writetxt()
#print("caiu")
#writegraph()
#print("caiu denovo")

while True:
    print("\nSelecione uma técnica:")
    print("1 - CYCLIC VOLTAMMETRY")
    print("2 - LINEAR SWEEP VOLTAMMETRY")
    print("3 - CHRONOAMPEROMETRY")
    

    # Espera pela entrada do usuário
    while True:
        try:
            selection = int(input("Digite sua escolha: "))
            if selection in [1, 2, 3]:
                break
            else:
                print("Opção inválida! Tente novamente.")
        except ValueError:
            print("Entrada inválida! Tente novamente.")

    # Envia a seleção para o Arduino
    serialInst.write(str(selection).encode())
    
   
    if selection == 1:        
        selection = input("\nENTER SCAN RATE \nALLOWED RANGE: 1 -250 mV/s\n")
        serialInst.write(str(selection).encode())
        print("Scan rate:" + selection + "mV/s")

        selection = input("\nEnter Start potential \nALLOWED RANGE: -1.36 a +1.41 volts\n")
        serialInst.write(str(selection).encode())
        print("Start Potential:" + str(selection) + "V")

        selection = input("\nEnter End potential \nALLOWED RANGE: -1.36 a +1.41 volts\n")
        serialInst.write(str(selection).encode())
        print("End Potential:" + selection + "V\n")

        print("Aguarde")
        writetxt()
        writegraph()
        

            

        

    
   




