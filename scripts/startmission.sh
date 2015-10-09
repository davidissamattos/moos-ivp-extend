#!/bin/bash

#Selecting arguments from the command line
while [[ $# > 1 ]]
do
key="$1"
case $key in
    -m|--mission)
    MISSION="$2"
    shift # past argument
    ;;
    -p|--path)
    PATH="$2"
    shift # past argument
    ;;
    -h|--help)
   	echo "To use this mission launcher you have to provide the mission file and the path"
    echo "Example ./startmission -path=~/moos-ivp-extend/missions -mission=alpha.moos"
	shift # past argument
    ;;
    --default)
    DEFAULT=YES
    ;;
    *)
            # unknown option
    ;;
esac
shift # past argument or value
done




