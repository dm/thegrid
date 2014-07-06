#!/usr/bin/env python
# -*- coding: utf-8 -*- #
from __future__ import unicode_literals

AUTHOR = 'Dr House Industries'
SITENAME = 'The·Grid'
SITEURL = ''

PATH = 'content'
STATIC_PATHS = ['images', 'extra/CNAME', 'extra/links.css',
                'extra/favicon.ico']
EXTRA_PATH_METADATA = {
    'extra/CNAME': {'path': 'CNAME'},
    'extra/links.css': {'path': 'links.css'},
    'extra/favicon.ico': {'path': 'favicon.ico'},
}

PLUGIN_PATHS=['plugins']
PLUGINS=['pin_to_top'] # For sticky posts

TIMEZONE = 'Europe/London'
DEFAULT_LANG = 'en'

TYPOGRIFY = True # Make quotes look pretty

# Feed generation is usually not desired when developing
FEED_ALL_ATOM = None
CATEGORY_FEED_ATOM = None
TRANSLATION_FEED_ATOM = None

DISPLAY_PAGES_ON_MENU = True
DEFAULT_PAGINATION = False
SUMMARY_MAX_LENGTH = 300

THEME = "themes/pelican-bootstrap3"
BOOTSTRAP_THEME = "cyborg"





# Uncomment following line if you want document-relative URLs when developing
#RELATIVE_URLS = True

# Blogroll
#LINKS = (('Pelican', 'http://getpelican.com/'),
#         ('Python.org', 'http://python.org/'),
#         ('Jinja2', 'http://jinja.pocoo.org/'),
#         ('You can modify those links in your config file', '#'),)

# Social widget
#SOCIAL = (('You can add links in your config file', '#'),
#          ('Another social link', '#'),)
