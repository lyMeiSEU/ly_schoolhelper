add_library('pdf')
import json
from tools import convert


def color(val):
    if val == 0:
        return 255,0,0
    if val == 1:
        return 255,42,0
    if val == 2:
        return 255,84,0
    if val == 3:
        return 255,126,0
    if val == 4:
        return 255,168,0
    if val == 5:
        return 255,210,0
    return 255,255,0
    
def line_color(val):
    if val == 0:
        return 0,0,255
    if val == 1:
        return 0,42,255
    if val == 2:
        return 0,84,255
    if val == 3:
        return 0,126,255
    if val == 4:
        return 0,168,255
    if val == 5:
        return 0,210,255
    return 0,255,255

def bus_color(val):
    if val == 0:
        return 255,0,0,128
    if val == 1:
        return 255,42,0,128
    if val == 2:
        return 255,84,0,128
    if val == 3:
        return 255,126,0,128
    if val == 4:
        return 255,168,0,128
    if val == 5:
        return 255,210,0,128
    return 255,255,0,128

def setup():
    beginRecord(PDF, "together-white-background.pdf"); 
    size(2400,1200)
    sz_boundary = []
    fp =  open('shenzhen.geojson')
    data = json.load(fp)
    for coor in data['features'][0]['geometry']['coordinates'][0][0]:
        sz_boundary.append(convert(*coor))
    fp.close()
    background(255)
    # stroke(255,255,255)
    stroke(0,0,0)
    strokeWeight(5)
    #noFill()
    # fill(0)
    fill(255)
    beginShape()
    for coor in sz_boundary:
        vertex(*coor)
    endShape()
    
    fp =  open('bus_station.geojson')
    strokeWeight(4)
    data = json.load(fp)
    for feature in data['features']:
        coor = convert(*feature['geometry']['coordinates'])
        val = feature['properties']['Contact']
        stroke(*color(val))
        point(*coor)
    fp.close()
    
    fp =  open('bus_line.geojson')
    strokeWeight(1)
    data = json.load(fp)
    for feature in data['features']:
        start_pt = feature['geometry']['coordinates'][0]
        end_pt = feature['geometry']['coordinates'][1]
        coor1 = convert(*start_pt)
        coor2 = convert(*end_pt)
        val = feature['properties']['Contact']
        stroke(*bus_color(val))
        line(coor1[0]-11,coor1[1]-8,coor2[0]-11,coor2[1]-8)
    fp.close()
    
    fp =  open('subway_lines.geojson')
    strokeWeight(4)
    data = json.load(fp)
    for feature in data['features']:
        start_pt = feature['geometry']['coordinates'][0]
        end_pt = feature['geometry']['coordinates'][1]
        coor1 = convert(*start_pt)
        coor2 = convert(*end_pt)
        val = feature['properties']['Contact']
        stroke(*line_color(val))
        line(coor1[0],coor1[1],coor2[0],coor2[1])
    fp.close()
    
    img = loadImage("public-transport-subway.png");
    fp =  open('subway_station.geojson')
    strokeWeight(1)
    data = json.load(fp)
    for feature in data['features']:
        coor = convert(*feature['geometry']['coordinates'])
        x = coor[0]-3
        y = coor[1]-3
        step = 10
        image(img, x, y,width/200,height/150);
    fp.close()
    
    
    
    endRecord();
    
    