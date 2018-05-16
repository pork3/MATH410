os="$(uname -s)"
case "${os}" in 
	Linux* ) machine=Linux;;
	CYGWIN*) machine=Cygwin;;
	Darwin*) machine=Mac;;
	*) machine=NA
esac
echo ${machine}

echo "Checking system information"
uname -a >> computerinfo.txt
echo "Checking cpu information"
lscpu >> computerinfo.txt
echo "Checking memory information"
cat /proc/meminfo >> computerinfo.txt
