from django.http import HttpResponse
from django.template import Context, Template
from django.template.loader import get_template
import datetime


def index(request):
    indx = get_template('index.html')
    #Render template
    c = Context()
    html = indx.render(c)
    return HttpResponse(html)

def camera(request):
    t = get_template('camera.html')
    #Render template
    c = Context()
    html = t.render(c)
    return HttpResponse(t)
    
def variables(request):
    t = get_template('variables.html')
    #Render template
    c = Context()
    html = t.render(c)
    return HttpResponse(t)
    
def info(request):
    t = get_template('info.html')
    #static page but renders
    c = Context()
    html = t.render(c)
    return HttpResponse(html)