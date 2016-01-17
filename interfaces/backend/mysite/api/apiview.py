from django.http import HttpResponse
from django.template import Context, Template
from django.template.loader import get_template

# Create your views here.

def helloworld(request):
    return HttpResponse("Hello World")