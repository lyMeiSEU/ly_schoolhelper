### 发布
优化了一下发布
```shell script
ssh root@62.234.134.210
Seu@Zimotion6102
cd /root/dev/alumni_server/
#测试环境
sh start.sh
#生产环境
sh start-prd.sh

```
// TODO CICD

### 备忘

```shell script
#查看日志 /root/dev/alumni_server/
tail -f nohup.out
```

```查看nginx配置
vim /etc/nginx/conf.d/weapp.conf

```

服务器域名：https://www.seuclab.cn

#项目资料

UI：https://lanhuapp.com/url/hdUEK-VtbqL
接口文档：https://nei.netease.com/dashboard/

生产环境：https://www.seuclab.cn/v2/，数据库：alumnidb_prd;
开发&测试环境：https://www.seuclab.cn/test/v2/，数据库：alumnidb_dev