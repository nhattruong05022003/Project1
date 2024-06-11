# Simple Project transmits data between Node and Gateway using LoRa

System will measure local envoriment at Node and then send the data to Gateway using LoRa. Gateway will upload the data to Cloud to display on the website. Each Node will be an access point so you can connect its WiFi to turn on / off the sensor or set the time between each sampling.

# Components:
- STM32F103C8T6 x 1

  ![image](https://github.com/nhattruong05022003/Project1/assets/145974955/c19e46cb-ab02-439e-b232-aa7d1bfba1e7)
 
- ESP32 x 2

  ![image](https://github.com/nhattruong05022003/Project1/assets/145974955/1c04da16-2244-45f7-85e6-fad05c94bae1)

- LoRa SX1278 x 2

  ![image](https://github.com/nhattruong05022003/Project1/assets/145974955/3d2b2dc5-2e87-4a6d-961a-5efe47b64f41)

  ![image](https://github.com/nhattruong05022003/Project1/assets/145974955/45d3c62f-6681-4d45-b1d9-7e6c570d86c0)

- SHT31 x 1

  ![image](https://github.com/nhattruong05022003/Project1/assets/145974955/0898d755-6018-48de-9693-0fe8ba5b9a57)

- BMP180 x 1

  ![image](https://github.com/nhattruong05022003/Project1/assets/145974955/f5b519e0-82d0-4cbc-88f5-08661fcd6113)

- LCD1602 x 1

  ![image](https://github.com/nhattruong05022003/Project1/assets/145974955/a41ce76b-82e3-4e1e-80dc-d63d5955b968)

# Schematic
- Node:

  STM32F103C8T6 interfaces with SHT31(sensor 1) and BMP180(sensor 2)
  
  ![image](https://github.com/nhattruong05022003/Project1/assets/145974955/5d0ab514-c0c1-49e3-855f-1606659e5e23)

  STM32F103C8T6 interfaces with LCD1602

  ![image](https://github.com/nhattruong05022003/Project1/assets/145974955/8d2992a3-b32f-44d1-bff5-83a041f2f509)

  STM32F103C8T6 interfaces with ESP32 and LoRa SX1278

  ![image](https://github.com/nhattruong05022003/Project1/assets/145974955/0c0a1745-264b-4dfd-8d19-b2e93c2548a2)

- Gateway:

  ![image](https://github.com/nhattruong05022003/Project1/assets/145974955/2b21b7a2-70f0-4178-9a53-a00281fb5b90)

# Overview
- Node:

  ![image](https://github.com/nhattruong05022003/Project1/assets/145974955/0074385a-f7ea-4da7-bd45-ce66ae76366f)

- Gateway:

  ![image](https://github.com/nhattruong05022003/Project1/assets/145974955/16dd219e-8598-42b2-9194-f6d2165d6949)

- Website:

  ![image](https://github.com/nhattruong05022003/Project1/assets/145974955/ba184fdf-dc6e-43a3-bfa8-383c629c30ab)

  ![image](https://github.com/nhattruong05022003/Project1/assets/145974955/cf9320d1-5f20-4ef6-b0d7-1960de51fcb9)

  ![image](https://github.com/nhattruong05022003/Project1/assets/145974955/a3cedd32-aae7-4116-8c17-7b097d6c71ca)
  
- Access point

  ![image](https://github.com/nhattruong05022003/Project1/assets/145974955/9c31a4a1-bf04-4743-aee7-63fae2e91167)
