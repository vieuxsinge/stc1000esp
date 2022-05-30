#!/bin/sh

((avahi-browse _arduino._tcp --resolve --parsable --terminate) 2>/dev/null | grep -F "=;") | cut -d\; -f4,8 | sort | uniq
