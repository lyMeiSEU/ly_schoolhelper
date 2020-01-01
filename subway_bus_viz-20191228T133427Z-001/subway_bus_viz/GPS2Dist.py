import math


def gps2lon(lon):
    return gps2Dist(lon, 0)


def gps2lat(lat):
    return gps2Dist(0, lat)

def gps2Dist(lon, lat):
    return getDist(lon, lat, 0, 0)

def getDist(lon1, lat1, lon2, lat2):
    if lon1 == lon2 and lat1 == lat2:
        return 0.0
    theta = lon1 - lon2
    dist = math.sin(deg2rad(lat1)) * math.sin(deg2rad(lat2)) + math.cos(deg2rad(lat1)) * math.cos(deg2rad(lat2)) * math.cos(deg2rad(theta))
    if dist > 1.0:
        dist = 1.0
    dist = rad2deg(math.acos(dist)) * 60 * 1.1515 * 1.609344
    return dist

def deg2rad(deg):
    return deg * math.pi / 180.0


def rad2deg(rad):
    return rad * 180.0 / math.pi