git pull
mvn clean package

#
#pkill -f alumni
#
port=18081
#根据端口号查询对应的pid
pid=$(netstat -nlp | grep :$port | awk '{print $7}' | awk -F"/" '{ print $1 }');
#杀掉对应的进程，如果pid不存在，则不执行
if [  -n  "$pid"  ];  then
    kill  -9  $pid;
fi
nohup java -jar target/alumni_server-0.0.1-SNAPSHOT.jar &