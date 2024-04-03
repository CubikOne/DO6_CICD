## Part 1. Настройка gitlab-runner  

1) устанавливаем gitlab-runner  

![alt text](image.png)


2) зарегистрируем  

![alt text](image-1.png) 
![alt text](image-2.png)


## Part 2. Сборка  


1) Напишем .yml файл для описания стадии сборки приложения C2  

![alt text](image-3.png)

2) На гит-лабе в CI/CD посмотрим что сборка прошла успешна  

![alt text](image-4.png)


## Part 3. Тест кодстайла
1) Дополним файл .yml тестом на код стайл  

![alt text](image-5.png)

2) В случае успеха  

![alt text](image-7.png)

3) В случае фейла  

![alt text](image-6.png)


## Part 4. Интеграционные тесты  


1) Дополним файл .yml для запуска тестов  

![alt text](image-8.png)


2) Этапы проходят успешно  

![alt text](image-9.png)


## Part 5. Этап деплоя

1) Заходим на первой машине под пользователя gitlab-runner(sudo su gitlab-runner), создаём ключ и копируем его на вторую машину(ssh-copy-id deploy@192.168.0.12)  
![alt text](image-10.png)  


2) Спустя миллион попыток, деплоем наши утилиты на вторую машину  
![alt text](image-11.png)  
![alt text](image-12.png)  
![alt text](image-13.png)  