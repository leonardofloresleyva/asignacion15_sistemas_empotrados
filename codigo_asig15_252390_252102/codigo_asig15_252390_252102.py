import serial

BAUD_RATE = 115200
esp32 = serial.Serial('COM3', BAUD_RATE, timeout=5)

while True:
    print('Para encender Leds de derecha a izquierda, teclea el comando "rtl"')
    print('Para encender Leds de izquierda a derecha, teclea el comando "ltr"')
    print('Para apagar los Leds, teclea el comando "off"')
    print('Para salir, teclea "exit"')
    print()
    comando = input('Comando deseado? ')
    print()

    if comando == 'exit':
        break

    if comando == 'ltr':
        esp32.write(b'ltr\n')
        
    elif comando == 'rtl':
        esp32.write(b'rtl\n')
        
    elif comando == 'off':
        esp32.write(b'off\n')
        
    else:
        print('Comando desconocido (error)')
        esp32.write(b'error\n')

    data = esp32.readline()[:-2]
    
    if data:
        sdata = data.decode('utf-8')
        print(sdata)