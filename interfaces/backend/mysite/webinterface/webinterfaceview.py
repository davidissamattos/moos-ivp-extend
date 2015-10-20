from django.http import HttpResponse
from django.template import Context, Template
from django.template.loader import get_template
import datetime
import moos
from boat import boat


def CameraView(request):
    t = get_template('CameraView.html')
    #Render template
    c = Context({'status_bar': 'Camera View',
        
    })
    html = t.render(c)
    return HttpResponse(html)

def VehicleCharacteristics(request):
    t = get_template('VehicleCharacteristics.html')
    #Render template
    c = Context({'status_bar': 'Vehicle Characteristics',
                'width': boat.width,
                'depth': boat.depth,
                'height': boat.height,
                'mass': boat.mass,
                'router': boat.router,
                'imu': boat.imu,
                'gps': boat.gps,
                'computer': boat.computer,
                'PowerControl': boat.PowerControl,
                })
    html = t.render(c)
    return HttpResponse(html)
    
def Dashboard(request):
    mymoos = moos.moosjson("shoreside.json")
    if mymoos.boolRead == True:
        t = get_template('Dashboard.html')
        #Render template 
        c = Context({'status_bar': 'Dashboard',
                     'variables': mymoos.variables,
                     'vehicle_name': mymoos.community,
                     'moos_time': mymoos.moostime,
                     })
    else:
        c = Context({'status_bar': 'Dashboard',})
        t = get_template('ErrorJSON.html')
   
    html = t.render(c)
    return HttpResponse(html)
    
def About(request):
    t = get_template('About.html')
    #static page but renders
    c = Context({'status_bar': 'About'
        
    })
    html = t.render(c)
    return HttpResponse(html)