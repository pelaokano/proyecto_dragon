CREATE DATABASE IF NOT EXISTS opcua_db;

USE opcua_db;

CREATE TABLE IF NOT EXISTS capacidad (
    id INT AUTO_INCREMENT PRIMARY KEY,
    capacidad_linea1 FLOAT NOT NULL,
    capacidad_linea2 FLOAT NOT NULL,
    capacidad_linea3 FLOAT NOT NULL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

INSERT INTO capacidad (capacidad_linea1, capacidad_linea2, capacidad_linea3) 
VALUES (1903.14994, 1921.630099, 1932.605749);