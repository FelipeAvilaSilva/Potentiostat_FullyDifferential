import datetime
import pandas as pd
import plotly.express as px

def create_plot(nomeArquivo):
    try:
        # Verifica se o arquivo é .txt
        if not nomeArquivo.endswith('.txt'):
            raise Exception('O arquivo precisa ter a extensão .txt')
        
        # Leitura do arquivo "hidrosens.txt" usando a biblioteca Pandas
        df = pd.read_csv(nomeArquivo, delimiter='[;,]', skipinitialspace=True, engine='python', header=None, names=['Potencial', 'Corrente'])
        
        # Criação de uma nova coluna de potencial para o gráfico
        df['Potencial'] = df['Potencial'].iloc[::-1].reset_index(drop=True)
        
        # Determina a mudança de cor com base na direção da varredura
        df['cores'] = 'blue'
        df.loc[df['Potencial'].diff() < 0, 'cores'] = 'red'
        
        # Criação do gráfico de voltametria cíclica usando a biblioteca Plotly
        fig = px.scatter(df, x='Potencial', y='Corrente', color='cores', title='Gráfico de Voltametria Cíclica')
        fig.update_layout(title_x=0.5)
        fig.update_xaxes(title_text='Potencial (V)')
        fig.update_yaxes(title_text='Corrente (uA)')
        fig.add_annotation(text=f'Gerado em {datetime.datetime.now().strftime("%d/%m/%Y %H:%M:%S")}', xref='paper', yref='paper', x=1, y=0, showarrow=False, font=dict(size=10))
        fig.show()
        
    except Exception as e:
        print(f'Ocorreu um erro ao gerar o gráfico: {str(e)}')

# Exemplo de uso
create_plot('hidrosens.txt')

