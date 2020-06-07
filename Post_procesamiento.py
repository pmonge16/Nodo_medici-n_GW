#Importar bibliotecas
import csv
import os

#Definición de variables globales
dict = {}
cant_datos = 0

def lector_lineas(archivo):
    frase = ''
    for row in archivo:
        for index in row:
            frase = index
            x = traductor(frase)
            generar_secuencia(x,salto)
            cant_datos += 1

def traductor(frase):
    try:
        print(frase," Traductor:", dict[frase])
        return dict[frase]
    except KeyError:
        print(frase)
        contenido = ''
        for c in range(0,len(frase)):
            contenido += frase[c]
            try:
                print(contenido," Traductor:",dict[contenido])
                return dict[contenido]
                break
            except KeyError:
                if( c > 1):
                    contenido = contenido[1:]
                continue
        return frase


def diccionario():
    num = 0
    for num in range(0,52):
        if num < 12:
            dict[str(23 + num)] = chr(97 + num)
        if (num > 11)&(num < 26):
            dict[str(43  + num)] = chr(97 + num)
        if num > 25:
            dict[str(43 + num)] = chr(39 + num)


def generar_secuencia(dato, salto):
    file = open("filename.txt", "a")
    if salto:
        file.write(dato + os.linesep)
    else:
        file.write(dato + ",")
    file.close()

if __name__ == '__main__':
    archivo = 'file.txt'
    with open(archivo) as csv_file:
        csv_lector = csv.reader(csv_file, delimiter=',')
        diccionario()
        lector_lineas(csv_lector)


