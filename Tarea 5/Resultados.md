# Presentación de resultados en transferencia TCP y UDP

Los resultados que se presentan a continuación se recopilaron de promediar tres corridas para cada uno de los códigos TCP/UDP. Siendo tres tablas con:

1. Envío de **10,000** paquetes de **1440 bytes** con un tamaño de buffer de **256 bytes** (*default*).
2. Envío de **10,000** paquetes de **1440 bytes** con un tamaño de buffer de **32*1024 bytes**.
3. Envío de **10,000** paquetes de **1440 bytes** con un tamaño de buffer de **5000*1024 bytes**.

<img src="https://raw.githubusercontent.com/RodolfoFerro/ComputerNetworks17/master/Tarea%205/server.png" width="50%"><img src="https://raw.githubusercontent.com/RodolfoFerro/ComputerNetworks17/master/Tarea%205/client.png" width="50%">

Dichos resultados se encuentran distribuidos en las siguientes tablas:

|                 |              |           TCP           |        TCP       |           UDP           |        UDP       |            UDP             |     UDP    |
| --------------- | ------------ | ----------------------- | ---------------- | ----------------------- | ---------------- | -------------------------- | ---------- |
| **Tam. buffer** | **Servidor** | **Tiempo de ejecución** | **Megabits/seg** | **Tiempo de ejecución** | **Megabits/seg** | **Paquetes perdidos (PP)** | **PP (%)** |
| *default* (256) | *localhost*  | 0.016939 seg            | 830184.780684    | X                       | X                | X                          |            |
| *default* (256) | Red local    | 0.029357 seg            | 479016.929523    | X                       | X                | X                          |            |


|                 |              |           TCP           |        TCP       |           UDP           |        UDP       |            UDP             |     UDP    |
| --------------- | ------------ | ----------------------- | ---------------- | ----------------------- | ---------------- | -------------------------- | ---------- |
| **Tam. buffer** | **Servidor** | **Tiempo de ejecución** | **Megabits/seg** | **Tiempo de ejecución** | **Megabits/seg** | **Paquetes perdidos (PP)** | **PP (%)** |
| 32*1024 bytes   | *localhost*  | 0.069634 seg            | 202009.986103    | X                       | X                | X                          |            |
| 32*1024 bytes   | Red local    | 0.323525 seg            | 43466.501816     | X                       | X                | X                          |            |


|                 |              |           TCP           |        TCP       |           UDP           |        UDP       |            UDP             |     UDP    |
| --------------- | ------------ | ----------------------- | ---------------- | ----------------------- | ---------------- | -------------------------- | ---------- |
| **Tam. buffer** | **Servidor** | **Tiempo de ejecución** | **Megabits/seg** | **Tiempo de ejecución** | **Megabits/seg** | **Paquetes perdidos (PP)** | **PP (%)** |
| 5000*1024 bytes | *localhost*  | 0.151377 seg            | 93803.343695     | X                       | X                | X                          |            |
| 5000*1024 bytes | Red local    | 0.332280 seg            | 42321.235103     | X                       | X                | X                          |            |


### Preguntas

1. Explique el por qué de sus resultados.
2. ¿Se perdieron paquetes en UDP? ¿Por qué?
3. ¿Cuál es mejor?
