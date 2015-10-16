To use is necessary to install mod_wsgi:
sudo pip install mod_wsgi
sudo pip2 install mod_wsgi
or
https://github.com/GrahamDumpleton/mod_wsgi

HARD WAY:
To run use (first configure django as in the site above):
python manage.py collectstatic
then
python manage.py runmodwsgi
or
python manage.py runmodwsgi --reload-on-changes

or configure as:
On Mac OS X
sudo python manage.py runmodwsgi --setup-only --port=80 --user David --group staff --server-root=/etc/mod_wsgi-express-80

and run using
/etc/mod_wsgi-express-80/apachectl start
/etc/mod_wsgi-express-80/apachectl restart
/etc/mod_wsgi-express-80/apachectl stop


EASY WAY:
add mod_wsgi.server to apps in django
python manage.py collectstatic
python manage.py runmodwsgi --reload-on-changes
    