import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Leitura do arquivo "hidrosens.txt" usando a biblioteca Pandas
df = pd.read_csv('hidrosens.txt', delimiter=';')

# Criação de uma nova coluna de potencial para o gráfico
df['potencial'] = df.iloc[:, 0]

# Inverter a ordem dos valores para que a tensão seja crescente
df['potencial'] = df['potencial'][::-1].reset_index(drop=True)

# Criação do gráfico de voltametria cíclica usando a biblioteca Matplotlib
plt.plot(df['potencial'], df.iloc[:, 1])
plt.xlabel('Potencial (V)')
plt.ylabel('Corrente (A)')
plt.title('Gráfico de Voltametria Cíclica')
plt.show()
