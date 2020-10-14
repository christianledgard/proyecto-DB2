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


source = ["disk_access_insert_sq.csv",
          "time_insert_sq.csv",
          "disk_access_search_sq.csv",
          "time_search_sq.csv"]

target = ["disk_access_insert_sq_n.csv",
          "time_insert_sq_n.csv",
          "disk_access_search_sq_n.csv",
          "time_search_sq_n.csv"]

for s, d in zip(source, target):
  generarSecuenciaCSV(s,d)


source = ["disk_access_delete_sq.csv",
          "time_delete_sq.csv"]

target = ["disk_access_delete_sq_n.csv",
          "time_delete_sq_n.csv"]

for s, d in zip(source, target):
  generarSecuenciaCSV(s,d,True)