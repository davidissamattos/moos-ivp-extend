from django.http import HttpResponse
from django.template import Context, Template
from django.template.loader import get_template
import datetime


def index(request):
    t = get_template('table.html')
    c = Context({'person_name': 'John Smith',  'company': 'Outdoor Equipment','ship_date': datetime.date(2009, 4, 2),'ordered_warranty': False})
    html = t.render(c)
    #Assert false
    return HttpResponse(html)


def current_datetime(request):
    now = datetime.datetime.now()
    html = "<html><body>It is now %s.</body></html>" % now
    return HttpResponse(html)
    
def helloweb(request):
    print 'A'
    return HttpResponse("Hello world web")
    

def hours_ahead(request, offset):
    try:
        offset = int(offset)
    except ValueError:
        raise Http404()
    dt = datetime.datetime.now() + datetime.timedelta(hours=offset)
    html = "<html><body>In %s hour(s), it will be %s.</body></html>" % (offset, dt)
    return HttpResponse(html)
    
def mytemplate(request):
    t = get_template('p1.html')
    c = Context({'person_name': 'John Smith',  'company': 'Outdoor Equipment','ship_date': datetime.date(2009, 4, 2),'ordered_warranty': False})
    html = t.render(c)
    #Assert false
    return HttpResponse(html)
    
def index(request):
    t = get_template('content.html')
    c = Context({'person_name': 'John Smith',  'company': 'Outdoor Equipment','ship_date': datetime.date(2009, 4, 2),'ordered_warranty': False})
    html = t.render(c)
    #Assert false
    return HttpResponse(html)