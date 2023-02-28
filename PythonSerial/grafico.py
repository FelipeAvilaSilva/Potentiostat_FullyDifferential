import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import datetime as dt

# Leitura do arquivo "hidrosens.txt" usando a biblioteca Pandas
df = pd.read_csv('hidrosens.txt', delimiter=';')

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
ax.set_ylabel('Corrente (A)')
ax.set_title('Gráfico de Voltametria Cíclica')
plt.figtext(0.99, 0.01, f'Gerado em {dt.datetime.now().strftime("%d/%m/%Y %H:%M:%S")}', horizontalalignment='right')
plt.show()

