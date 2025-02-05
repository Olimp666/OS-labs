document.addEventListener('DOMContentLoaded', function () {
    const script = document.createElement('script');
    script.src = 'https://cdn.jsdelivr.net/npm/chart.js';
    document.head.appendChild(script);
});

async function fetchData() {
    try {
        const response = await fetch('/current');
        const data = await response.text();
        console.log(data)
        document.getElementById('currentTemp').textContent = parseFloat(data).toFixed(2)+'°C';
    } catch (error) {
        document.getElementById('currentTemp').textContent = 'Error loading data';
        console.error('Error:', error);
    }
    try {
        const response = await fetch('/mean/hour');
        const data = await response.json();
        // console.log(data)
        const labels = data.map(item => {
            const startTime = new Date(item.time * 1000);  // Начальное время
            const endTime = new Date((item.time + 3600) * 1000);  // Время через 1 час (3600 секунд)

            // Формируем строку с временным промежутком
            const startTimeStr = startTime.toLocaleString();
            const endTimeStr = endTime.toLocaleString();
            return `${startTimeStr} - ${endTimeStr}`;
        });
        // console.log(labels)
        const ctx = document.getElementById('hourMean').getContext('2d');
        const myChart = new Chart(ctx, {
            type: 'bar',  // Тип графика (гистограмма)
            data: {
                labels: labels,  // Метки по оси X
                datasets: [{
                    label: 'Температура',
                    data: data.map(item => { // Преобразуем unix time в миллисекунды
                        return item.temperature;  // Форматируем дату
                    }),  // Данные для оси Y
                    backgroundColor: 'rgba(54, 162, 235, 0.2)',  // Цвет баров
                    borderColor: 'rgba(54, 162, 235, 1)',  // Цвет границы баров
                    borderWidth: 1
                }]
            },
            options: {
                scales: {
                    y: {
                        beginAtZero: true  // Начинать шкалу с 0
                    }
                }
            }
        });
    } catch (error) {
        // document.getElementById('hourMean').textContent = 'Error loading data';
        console.error('Error:', error);
    }
    try {
        const response = await fetch('/mean/day');
        const data = await response.json();
        // console.log(data)
        const labels = data.map(item => {
            const startTime = new Date(item.time * 1000);  // Начальное время
            const endTime = new Date((item.time + 3600*24) * 1000);  // Время через 1 час (3600 секунд)

            // Формируем строку с временным промежутком
            const startTimeStr = startTime.toLocaleDateString();
            const endTimeStr = endTime.toLocaleDateString();
            return `${startTimeStr} - ${endTimeStr}`;
        });
        // console.log(labels)
        const ctx = document.getElementById('dayMean').getContext('2d');
        const myChart = new Chart(ctx, {
            type: 'bar',  // Тип графика (гистограмма)
            data: {
                labels: labels,  // Метки по оси X
                datasets: [{
                    label: 'Температура',
                    data: data.map(item => { // Преобразуем unix time в миллисекунды
                        return item.temperature;  // Форматируем дату
                    }),  // Данные для оси Y
                    backgroundColor: 'rgba(54, 162, 235, 0.2)',  // Цвет баров
                    borderColor: 'rgba(54, 162, 235, 1)',  // Цвет границы баров
                    borderWidth: 1
                }]
            },
            options: {
                scales: {
                    y: {
                        beginAtZero: true  // Начинать шкалу с 0
                    }
                }
            }
        });
    } catch (error) {
        // document.getElementById('hourMean').textContent = 'Error loading data';
        console.error('Error:', error);
    }
}

// Call the function when page loads
fetchData();