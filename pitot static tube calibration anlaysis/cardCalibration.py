# -*- coding: utf-8 -*-
"""
Created on Tue Apr 30 14:48:22 2019

@author: phil
"""


import pandas as pd

## Read data from file 'filename.csv' 
## (in the same directory that your python process is based)
## Control delimiters, rows, column names with read_csv (see later) 
#data_s = pd.read_csv("cardCalibration2_2019-04-30T18-44-49.982Z_dur30_size1_Sensors.csv") 
#data_t = pd.read_csv("cardCalibration2_2019-04-30T18-44-49.982Z_dur30_size1_Tunnel.csv") 
## Preview the first 5 lines of the loaded data 
#data_s.head()
#data_t.head()
#
#transitionPoints = {}
#transitionPoints[0] = {}
#transitionPoints[0]['startingIndex'] = 0
#transitionPoints[0]['time'] = data_t.iloc[0].timestamp
#
#transitionPoints[101] = {}
#transitionPoints[101]['startingIndex'] = len(data_t.index)
#transitionPoints[101]['time'] = data_t.iloc[len(data_t.index)-1].timestamp
#
#for index, row in data_t.iterrows():
#     #print(index, row)
#     if (data_t.iloc[index].percentSetting > data_t.iloc[index-1].percentSetting) :
#         #print(index, row['timestamp'], row['percentSetting'])
#         transitionPoints[row['percentSetting']] = {}
#         transitionPoints[row['percentSetting']]['startingIndex'] = index
#         transitionPoints[row['percentSetting']]['time'] = row['timestamp']
#
#
#df = pd.DataFrame(transitionPoints).transpose()

import csv
import numpy as np


with open('cardCalibration2_2019-04-30T18-44-49.982Z_dur30_size1_Tunnel.csv', 'r') as f:
    tunnel_source = list(csv.reader(f, delimiter=','))
      #print(tunnel[:2])
with open('cardCalibration2_2019-04-30T18-44-49.982Z_dur30_size1_Sensors.csv', 'r') as f:
    sensors_source = list(csv.reader(f, delimiter=','))
      
tunnel = np.array(tunnel_source[1:], dtype=np.float)
sensors = np.array(sensors_source[1:], dtype=np.float)


transitionPoints = {}
transitionPoints[0] = {}
transitionPoints[0]['startingIndex'] = 0
transitionPoints[0]['startingTime'] = tunnel[0][0]

transitionPoints[101] = {}
transitionPoints[101]['startingIndex'] = len(tunnel)
transitionPoints[101]['startingTime'] = tunnel[len(tunnel)-1][0]

for index in range(len(tunnel)):
    if (tunnel[index][1] > tunnel[index-1][1]) :
        #print(index, row['timestamp'], row['percentSetting'])
        transitionPoints[tunnel[index][1]] = {}
        transitionPoints[tunnel[index][1]]['startingIndex'] = index
        transitionPoints[tunnel[index][1]]['startingTime'] = tunnel[index][0]

for indexPWM in range(len(transitionPoints)-1):
    transitionPoints[indexPWM]['middleTime'] = (transitionPoints[indexPWM+1]['startingTime']-transitionPoints[indexPWM]['startingTime'])/2+transitionPoints[indexPWM]['startingTime']
    transitionPoints[indexPWM]['calibrationTimeStart'] = transitionPoints[indexPWM]['middleTime'] - 1
    transitionPoints[indexPWM]['calibrationTimeEnd'] = transitionPoints[indexPWM]['middleTime'] + 1


for indexPWM in range(len(transitionPoints)-1):
    transitionPoints[indexPWM]['calibrationSensorIndexStart'] = np.searchsorted(sensors[:,0], transitionPoints[indexPWM]['calibrationTimeStart'])
    transitionPoints[indexPWM]['calibrationSensorIndexEnd'] = np.searchsorted(sensors[:,0], transitionPoints[indexPWM]['calibrationTimeEnd'])
    transitionPoints[indexPWM]['avgPitotPressureZero'] = np.average(sensors[transitionPoints[indexPWM]['calibrationSensorIndexStart']:transitionPoints[indexPWM]['calibrationSensorIndexEnd'],1])
    transitionPoints[indexPWM]['avgPitotPressureRaw'] = np.average(sensors[transitionPoints[indexPWM]['calibrationSensorIndexStart']:transitionPoints[indexPWM]['calibrationSensorIndexEnd'],2])
    transitionPoints[indexPWM]['avgPitotTemperature'] = np.average(sensors[transitionPoints[indexPWM]['calibrationSensorIndexStart']:transitionPoints[indexPWM]['calibrationSensorIndexEnd'],3])
    transitionPoints[indexPWM]['avgPitotVelocity'] = np.average(sensors[transitionPoints[indexPWM]['calibrationSensorIndexStart']:transitionPoints[indexPWM]['calibrationSensorIndexEnd'],4])
    transitionPoints[indexPWM]['avgHotWireTemperature'] = np.average(sensors[transitionPoints[indexPWM]['calibrationSensorIndexStart']:transitionPoints[indexPWM]['calibrationSensorIndexEnd'],5])
    transitionPoints[indexPWM]['avgHotWireVelocity'] = np.average(sensors[transitionPoints[indexPWM]['calibrationSensorIndexStart']:transitionPoints[indexPWM]['calibrationSensorIndexEnd'],6])
    transitionPoints[indexPWM]['avgPitotCalculatedVelocity'] = 14.4665769723*np.sqrt((transitionPoints[indexPWM]['avgPitotTemperature']+273.15)*transitionPoints[indexPWM]['avgPitotPressureZero'])
    transitionPoints[indexPWM]['avgPitotCalculatedVelocityRaw'] = 14.4665769723*np.sqrt((transitionPoints[indexPWM]['avgPitotTemperature']+273.15)*transitionPoints[indexPWM]['avgPitotPressureRaw'])

#remove temporary 101% entry
del transitionPoints[101]

#convert to dataframe for excel export
output = pd.DataFrame(transitionPoints).transpose()
output.to_excel("cardCalibrationOutput.xlsx")






























