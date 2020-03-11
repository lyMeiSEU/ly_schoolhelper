package cn.edu.seu.alumni_server.dataSync.config;

import com.alibaba.druid.pool.DruidDataSource;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import javax.sql.DataSource;

@Configuration
public class DruidCfgV1 {

    @Value("${v1.db.mysql.url}")
    String url;
    @Value("${db.mysql.username}")
    String username;
    @Value("${db.mysql.password}")
    String password;

    @Bean
    public DataSource dataSourceV1() {
        DruidDataSource ds = new DruidDataSource();
        ds.setUrl(url);
        ds.setUsername(username);
        ds.setPassword(password);

        ds.setValidationQuery("select 1");
        ds.setTestWhileIdle(true);
        ds.setTestOnBorrow(true);

        // 配置初始化大小、最小、最大
        ds.setInitialSize(1);
        ds.setMinIdle(10);
        ds.setMaxActive(30);
        // 配置获取连接等待超时的时间
//        ds.setMaxWait(60 * 1000);

        return ds;
    }
}
