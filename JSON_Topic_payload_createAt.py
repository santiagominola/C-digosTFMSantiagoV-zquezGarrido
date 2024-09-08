import json

# Nombre del archivo JSON de entrada
input_filename = 'A0SensMagA1SVK1F_EscCasa.json'
# Nombre del archivo de salida
output_filename = 'A0SensMag1F_EscCasa_payload_createAt.json'
# Topic deseado
topic_deseado = 'sensor/loggerA0'

# Leer el archivo JSON de entrada
with open(input_filename, 'r') as file:
    mensajes = json.load(file)

# Lista para almacenar los datos extraídos
datos_extraidos = []

# Extraer los campos "payload" y "createAt" de cada mensaje si el topic es el deseado
for mensaje in mensajes:
    if mensaje.get("topic") == topic_deseado:
        dato = {
            "payload": mensaje.get("payload"),
            "createAt": mensaje.get("createAt")
        }
        datos_extraidos.append(dato)

# Escribir los datos extraídos en el archivo de salida
with open(output_filename, 'w') as file:
    json.dump(datos_extraidos, file, indent=4)

print(f"Datos extraídos y guardados en {output_filename}")
