from django.http import HttpResponse
from django.template import Context, Template
from django.template.loader import get_template
import datetime
import moos


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
        
    })
    html = t.render(c)
    return HttpResponse(html)
    
def Dashboard(request):
    t = get_template('Dashboard.html')
    #Render template
    mymoos = moos.moosjson("shoreside.json")
    c = Context({'status_bar': 'Dashboard',
                 'variables': mymoos.variables,
                 'vehicle_name': mymoos.community,
                 'moos_time': mymoos.moostime,
    })
    html = t.render(c)
    return HttpResponse(html)
    
def About(request):
    t = get_template('About.html')
    #static page but renders
    c = Context({'status_bar': 'About'
        
    })
    html = t.render(c)
    return HttpResponse(html)