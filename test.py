C1 = 40127
C2 = 36924
C3 = 23317
C4 = 23282
C5 = 33464
C6 = 28312


def convert(D1, D2):
    dT = D2 - (C5 << 8)
    TEMP = 2000 + ((dT*C6) >> 23)

    OFF = (C2 << 16) + ((C4*dT) >> 7)
    SENS = (C1 << 15) + ((C3*dT) >> 8)

    T2 = 0
    OFF2 = 0
    SENS2 = 0

    if TEMP < 2000:
        T2 = (dT**2) >> 31
        OFF2 = (5*(TEMP-2000)**2) >> 1
        SENS2 = OFF2 >> 1

        if TEMP < -1500:
            OFF2 += 7*(TEMP+1500)**2
            SENS2 += 11*(TEMP+1500)**2 >> 1

    TEMP -= T2
    OFF -= OFF2
    SENS -= SENS2

    P = (((D1*SENS) >> 21) - OFF) >> 15

    return TEMP, P


lst = ((9085466, 8569150),  # From datasheet
       (9085466, 6869150),
       (4265350, 6269150),
       (4265350, 7598340),
       (6087840, 7990420),
       (8894340, 8190420),
       (9764720, 9390420),
       )

for i, (D1, D2) in enumerate(lst):
    print("---------------------------")
    print(i)
    print("D1: {}\tD2: {}".format(D1, D2))
    t, p = convert(D1, D2)
    print("Pres: {}\tTemp: {}".format(p, t))
