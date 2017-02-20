#!/bin/sh
cat $1 | sed 's/\./Ook! Ook. /g' | sed 's/>/Ook. Ook? /g' | sed 's/</Ook? Ook. /g' | sed 's/+/Ook. Ook. /g' | sed 's/-/Ook! Ook! /g' | sed 's/,/Ook. Ook! /g' | sed 's/\[/Ook! Ook? /g' | sed 's/\]/Ook? Ook! /g'
