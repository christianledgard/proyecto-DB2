import pandas as pd

def generarSecuenciaCSV(nombreArchivoSource, nombreArchivoDesnation, delete = False):
  df = pd.read_csv(nombreArchivoSource, header=None)

  lista = []
  lista.append(0)
  temporal = 1
  flag = False
  for i in range(9999):
    lista.append(temporal)
    if(flag):
      flag = False
      temporal += 1
    else:
      flag = True

  if(delete):
    lista.reverse()

  df['n'] = lista
  df.to_csv(nombreArchivoDesnation, header=None, index=None)


source = ["memory_insert_hash.csv",
          "time_insert_hash.csv",
          "memory_search_hash.csv",
          "time_search_hash.csv"]

target = ["memory_insert_hash_n.csv",
          "time_insert_hash_n.csv",
          "memory_search_hash_n.csv",
          "time_search_hash_n.csv"]

for s, d in zip(source, target):
  generarSecuenciaCSV(s,d)


source = ["memory_remove_hash.csv",
          "time_remove_hash.csv"]

target = ["memory_remove_hash_n.csv",
          "time_remove_hash_n.csv"]

for s, d in zip(source, target):
  generarSecuenciaCSV(s,d,True)