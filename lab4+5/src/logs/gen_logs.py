import time
from datetime import datetime
from collections import defaultdict
import random
import os

def generate_logs(start_date_str, filename="gen/second_file.log"):
    # Преобразуем выбранную дату в datetime объект
    start_date = datetime.strptime(start_date_str, "%Y-%m-%d %H:%M:%S")
    
    # Получаем текущую дату и время
    current_time = datetime.now()

    # Проверяем, что выбранная дата не позже текущей
    if start_date > current_time:
        print("Выбранная дата должна быть не позже текущего времени.")
        return

    # Начинаем с выбранной даты
    timestamp = int(start_date.timestamp())
    
    # Открываем файл для записи
    with open(filename, 'w') as file:
        # Генерируем логи с шагом 1 секунда
        while timestamp <= int(current_time.timestamp()):
            temperature = round(random.uniform(10, 20), 2)
            file.write(f"{timestamp} {temperature}\n")

            # Переходим к следующей секунде
            timestamp += 1

    print(f"Логи успешно сохранены в файл {filename}")

def calculate_hourly_averages(input_filename="gen/second_file.log", output_filename="gen/hourly_file.log"):
    # Словарь для хранения температур по часам
    hourly_data = defaultdict(list)

    # Чтение данных из файла
    with open(input_filename, 'r') as file:
        for line in file:
            # Разделяем строку по символу "-" для получения timestamp и температуры
            timestamp_str, temperature_str = line.strip().split(' ')
            timestamp = int(timestamp_str)
            temperature = float(temperature_str)
            
            # Преобразуем timestamp в объект datetime
            dt = datetime.fromtimestamp(timestamp)
            
            # Извлекаем только начало часа (поле hour)
            hour_start = dt.replace(minute=0, second=0, microsecond=0)
            
            # Сохраняем температуру в соответствующем часе
            hourly_data[hour_start].append(temperature)

    # Запись средних температур по часам в новый файл
    with open(output_filename, 'w') as output_file:
        for hour_start in sorted(hourly_data.keys()):
            # Вычисляем среднее значение температуры для этого часа
            avg_temp = sum(hourly_data[hour_start]) / len(hourly_data[hour_start])
            # Записываем в файл
            output_file.write(f"{int(hour_start.timestamp())} {avg_temp:.2f}\n")
    
    print(f"Средние температуры по часам записаны в файл {output_filename}")

def calculate_daily_averages(input_filename="gen/second_file.log", output_filename="gen/daily_file.log"):
    # Словарь для хранения температур по дням
    daily_data = defaultdict(list)

    # Чтение данных из файла
    with open(input_filename, 'r') as file:
        for line in file:
            # Разделяем строку по символу "-" для получения timestamp и температуры
            timestamp_str, temperature_str = line.strip().split(' ')
            timestamp = int(timestamp_str)
            temperature = float(temperature_str)
            
            # Преобразуем timestamp в объект datetime
            dt = datetime.fromtimestamp(timestamp)
            
            # Извлекаем только дату (убираем время)
            day_start = dt.replace(hour=0, minute=0, second=0, microsecond=0)
            
            # Сохраняем температуру в соответствующем дне
            daily_data[day_start].append(temperature)

    # Запись средних температур по дням в новый файл
    with open(output_filename, 'w') as output_file:
        for day_start in sorted(daily_data.keys()):
            # Вычисляем среднее значение температуры для этого дня
            avg_temp = sum(daily_data[day_start]) / len(daily_data[day_start])
            # Записываем в файл
            output_file.write(f"{int(day_start.timestamp())} {avg_temp:.2f}\n")
    
    print(f"Средние температуры по дням записаны в файл {output_filename}")


def ensure_gen_directory():
    if not os.path.exists('gen'):
        os.makedirs('gen')

ensure_gen_directory()
generate_logs("2025-01-15 00:00:00")
calculate_hourly_averages()
calculate_daily_averages()
