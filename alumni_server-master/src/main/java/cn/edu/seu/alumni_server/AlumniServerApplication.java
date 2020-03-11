package cn.edu.seu.alumni_server;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
//@tk.mybatis.spring.annotation.MapperScan(basePackages = "cn.edu.seu.alumni_server")
public class AlumniServerApplication {

    public static void main(String[] args) {
        SpringApplication.run(AlumniServerApplication.class, args);
    }

}
