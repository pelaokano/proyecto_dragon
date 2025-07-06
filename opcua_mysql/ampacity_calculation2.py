import numpy as np
import math
import time
import mysql.connector
import os

MYSQL_HOST = os.getenv("MYSQL_HOST", "mysql")
MYSQL_USER = os.getenv("MYSQL_USER", "opcua_user")
MYSQL_PASSWORD = os.getenv("MYSQL_PASSWORD", "opcua_pass")
MYSQL_DB = os.getenv("MYSQL_DB", "opcua_db")

# Funciones de calculo
def Tfilm_calc(Ts,Ta):
  Tfilm=(Ts+Ta)/2
  return Tfilm

def kf_calc(Tfilm):
  kf=7E-5*Tfilm+0.0242
  return kf

def mu_f_calc(Tfilm):
  return (1.458E-6*(Tfilm+273)**1.5)/(Tfilm+383.4)

def rho_f_calc(Tfilm,He):
  return (1.293-1.525E-4*He+6.379E-9*He**2)/(1+0.00367*Tfilm)

# Perdidas por conveccion
def qc_calc(phi,Do,rho_f,Vw,mu_f,kf,Ts,Ta):
  phi=np.pi*(phi/180)
  # Kangle=1.194-np.cos(phi)+0.194*np.cos(2*phi)+0.368*2*np.sin(2*phi)
  Kangle = 1.194 - np.cos(phi) + 0.194 * np.cos(2 * phi) + 0.368 * (np.sin(phi) ** 2)
  
  Nre=(Do*rho_f*Vw)/mu_f
  
  # print(f"phi {phi} Kangle {Kangle} Nre {Nre}")

  #Bajo Numero de Reynolds
  qc_1=Kangle*(1.01+1.35*Nre**0.52)*kf*(Ts-Ta)
  #Alto Numero de Reynolds
  qc_2=Kangle*0.754*Nre**0.6*kf*(Ts-Ta)

  # print(f"qc_1 {qc_1} qc_2 {qc_2}")

  #se selecciona el mayor entre qc1 y qc2
  if qc_1 > qc_2:
    qc=qc_1
  else:
    qc=qc_2
  return qc

# Perdida por calor irradiado    
def qr_calc(Do,em,Ts,Ta):
  return 17.8*Do*em*((((Ts+273)/100)**4)-((Ta+273)/100)**4)

# Tasa de ganancia por radiacion solar
def qs_calc(N,Lat,omega,Zl,He,alfa,Doo):
  delta=23.46*np.sin(np.pi*((((284+N)/365)*360)/180))
  delta=delta*np.pi/180
  
  Lat=(Lat*np.pi)/180
  omega=(omega*np.pi)/180
  
  Hc=np.arcsin(np.cos(Lat)*np.cos(delta)*np.cos(omega)+np.sin(Lat)*np.sin(delta))
  
  chi=np.sin(omega)/(np.sin(Lat)*np.cos(omega)-np.cos(Lat)*np.tan(delta))
  
  if (omega < 0) and (-180<=omega):
    if chi >=0:
      C=0
    else:
      C=180
  else:
    if chi >=0:
      C=180
    else:
      C=360
  
  Zc_grad=C+(np.arctan(chi)*180/np.pi)
  Zc=Zc_grad*np.pi/180
  
  Hc_grad=Hc*180/np.pi 

  #Atmosfera limpia
  A=-42.2391
  B=63.8044
  C=-1.922
  D=3.46921E-2
  E=-3.6118E-4
  F=1.94318E-6
  G=-4.07608E-9

  Qs=A+B*Hc_grad+C*Hc_grad**2+D*Hc_grad**3+E*Hc_grad**4+F*Hc_grad**5+G*Hc_grad**6
  
  Zl=Zl*np.pi/180  
  theta=np.arccos(np.cos(Hc)*np.cos(Zc-Zl))
  #theta_grad=theta*180/np.pi
  
  A=1
  B=1.148E-4
  C=1.108E-8
  ksolar=  A+B*He+C*He**2
  
  Qse=ksolar*Qs  
  return alfa*Qse*np.sin(theta)*Doo

def R_Ts_calc(R_Th,R_Tl,T_h,T_l,Ts):
  R=R_Tl+((R_Th-R_Tl)/(T_h-T_l))*(Ts-T_l)
  return R
  
def I_calc(qc,qr,qs,R_Ts):
  if ((qc+qr-qs)/R_Ts) <0:
    return 0
  else:
    return np.sqrt((qc+qr-qs)/R_Ts)


def calcular_corriente_actual(lat, lon,
                               Ts=80,           # Temperatura superficial asumida (°C)
                               He=500,          # Altura (m)
                               Lat_geo=36.99,   # Latitud del conductor
                               Zl=90.0,         # Azimut
                               Do=0.0346,       # Diámetro exterior (m)
                               em=0.5,          # Emisividad
                               alfa=0.5,        # Absorptividad
                               R_Th=0.0475,     # Resistencia a Th
                               R_Tl=0.0433,     # Resistencia a Tl
                               T_h=80,          # Th en °C
                               T_l=25,          # Tl en °C
                               R_ND=1,          # Número de subconductores
                               Ta=30,           # Temperatura ambiente
                               hora_actual=12): # Hora actual         
   

    Vw = 2.57
    phi = 340

    Tfilm = Tfilm_calc(Ts, Ta)
    kf = kf_calc(Tfilm)
    mu_f = mu_f_calc(Tfilm)
    rho_f = rho_f_calc(Tfilm, He)

    qc = qc_calc(phi, Do, rho_f, Vw, mu_f, kf, Ts, Ta)
    qr = qr_calc(Do, em, Ts, Ta)
    qs = qs_calc(R_ND, Lat_geo, hora_actual, Zl, He, alfa, Do)

    R_Ts = R_Ts_calc(R_Th, R_Tl, T_h, T_l, Ts)
    R_Ts_m = R_Ts / 1000  # Ω/m

    I = round(I_calc(qc, qr, qs, R_Ts_m), 6)


    return {
        "hora": hora_actual,
        "lat": round(lat, 6),
        "lon": round(lon, 6),
        "Ta": round(Ta, 6),
        "Vw": round(Vw, 6),
        "phi": round(phi, 6),
        "qc": round(qc, 6),
        "qr": round(qr, 6),
        "qs": round(qs, 6),
        "R_Ts (Ω/m)": round(R_Ts_m, 6),
        "I (A)": round(I, 6)
    }

def temperatura_por_hora(hora, T_min=5, T_max=35):

    hora_dia = (hora - 1) % 24
    
    if 6 <= hora_dia <= 18:
        fase = (hora_dia - 15) * (2 * math.pi / 24)
        temperatura = (T_max + T_min)/2 - ((T_max - T_min)/2) * math.cos(fase)
    else:
        if hora_dia < 6:
            temperatura = T_min + ( (6 - hora_dia)/6 ) * ( (T_max + T_min)/2 - T_min )
        else:
            temperatura = T_min + ( (hora_dia - 18)/6 ) * ( (T_max + T_min)/2 - T_min )
    
    return round(temperatura,3)

def actualizar_capacidad(valor1, valor2, valor3):
    conn = mysql.connector.connect(
        #host="localhost",  # o la IP del contenedor si no está en localhost
        #user="opcua_user",
        #password="opcua_pass",
        #database="opcua_db",

        host=MYSQL_HOST,
        user=MYSQL_USER,
        password=MYSQL_PASSWORD,
        database=MYSQL_DB,
        port=3306
    )
    cursor = conn.cursor()
    # Insertar nuevo valor
    #INSERT INTO capacidad (capacidad_linea1, capacidad_linea2, capacidad_linea3) 
    cursor.execute("INSERT INTO capacidad (capacidad_linea1, capacidad_linea2, capacidad_linea3) VALUES (%s, %s, %s)", (valor1,valor2,valor3))
    conn.commit()
    cursor.close()
    conn.close()
    # print(f"Valor {valor} insertado correctamente.")

def run():

    for hour in range(1, 8760):
        time.sleep(5)
        Ta = temperatura_por_hora(hour)
        # print(f"hora: {hour}, temperatura: {Ta}")
    
        resultado1 = calcular_corriente_actual(40.4168, -3.7038, Do=0.0346,R_Th=0.0475,R_Tl=0.0433,Ta=Ta,hora_actual=hour)
        capacidad_linea1 = resultado1['I (A)']

        resultado2 = calcular_corriente_actual(40.4168, -3.7038, Do=0.0320,R_Th=0.0440,R_Tl=0.042,Ta=Ta,hora_actual=hour)
        capacidad_linea2 = resultado2['I (A)']

        resultado3 = calcular_corriente_actual(40.4168, -3.7038, Do=0.0300,R_Th=0.0415,R_Tl=0.039,Ta=Ta,hora_actual=hour)
        capacidad_linea3 = resultado3['I (A)']

        #print(f"capacidad_linea1: {capacidad_linea1}, capacidad_linea2: {capacidad_linea2}, capacidad_linea3: {capacidad_linea3}")
        actualizar_capacidad(capacidad_linea1, capacidad_linea2, capacidad_linea3)
        

if __name__ == '__main__':
    
    for hour in range(1, 8760):
        time.sleep(5)
        Ta = temperatura_por_hora(hour)
        print(f"hora: {hour}, temperatura: {Ta}")
    
        resultado1 = calcular_corriente_actual(40.4168, -3.7038, Do=0.0346,R_Th=0.0475,R_Tl=0.0433,Ta=Ta,hora_actual=hour)
        capacidad_linea1 = resultado1['I (A)']

        resultado2 = calcular_corriente_actual(40.4168, -3.7038, Do=0.0320,R_Th=0.0440,R_Tl=0.042,Ta=Ta,hora_actual=hour)
        capacidad_linea2 = resultado2['I (A)']

        resultado3 = calcular_corriente_actual(40.4168, -3.7038, Do=0.0300,R_Th=0.0415,R_Tl=0.039,Ta=Ta,hora_actual=hour)
        capacidad_linea3 = resultado3['I (A)']

        print(f"capacidad_linea1: {capacidad_linea1}, capacidad_linea2: {capacidad_linea2}, capacidad_linea3: {capacidad_linea3}")
