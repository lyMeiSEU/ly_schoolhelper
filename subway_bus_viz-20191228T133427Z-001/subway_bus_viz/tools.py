from GPS2Dist import getDist,gps2lon,gps2lat

cur_min_x = 113.735223
cur_max_x = 114.649835
cur_min_y = 22.426765
cur_max_y = 22.845040


def contain(x,y,ps):
    result = 0
    j = len(ps)-1

    for i in range(0,len(ps)):
        if ((ps[i][1] > y) != (ps[j][1] > y)) and (x < (ps[j][0] - ps[i][0]) * (y - ps[i][1]) / (ps[j][1]-ps[i][1]) + ps[i][0]):
            result = ~result
        j = i
        i += 1
    return result


def convert(lon,lat):
    cur_min_x = 12635.0
    cur_max_x = 12755.0
    cur_min_y = 2485.0
    cur_max_y = 2545.0
    tar_min_x = 0
    tar_max_x = 2400
    tar_min_y = 0
    tar_max_y = 1200
    llon = gps2lon(lon)
    llat = gps2lat(lat)
    x = reflect(llon,cur_min_x,cur_max_x,tar_min_x,tar_max_x)
    y = tar_max_y - reflect(llat,cur_min_y,cur_max_y,tar_min_y,tar_max_y)
    return [x,y]

def reflect(val,cur_min,cur_max,tar_min,tar_max):

    tmp1 = (val - cur_min) / (cur_max - cur_min)
    tmp2 = tar_max - tar_min
    res = tar_min + tmp2*tmp1
    return int(round(res))