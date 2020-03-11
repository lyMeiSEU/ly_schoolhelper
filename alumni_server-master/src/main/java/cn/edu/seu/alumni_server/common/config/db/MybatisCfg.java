package cn.edu.seu.alumni_server.common.config.db;

import com.github.pagehelper.PageInterceptor;
import org.apache.ibatis.plugin.Interceptor;
import org.apache.ibatis.session.SqlSessionFactory;
import org.mybatis.spring.SqlSessionFactoryBean;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.io.support.PathMatchingResourcePatternResolver;
import org.springframework.core.io.support.ResourcePatternResolver;
import tk.mybatis.spring.mapper.MapperScannerConfigurer;

import javax.sql.DataSource;
import java.util.Properties;

@Configuration
public class MybatisCfg {
	@Bean(name = "sqlSessionFactory")
	public SqlSessionFactory sqlSessionFactoryBean(@Qualifier("dataSource") DataSource dataSource) throws Exception {
		SqlSessionFactoryBean bean = new SqlSessionFactoryBean();
		bean.setDataSource(dataSource);
		bean.setTypeAliasesPackage("cn.edu.seu.alumni_server.dao.entity");
		// 分页插件配置
		PageInterceptor pageHelper = new PageInterceptor();
		Properties properties = new Properties();
		properties.setProperty("reasonable", "true");
		properties.setProperty("params", "count");
		pageHelper.setProperties(properties);
		bean.setPlugins(new Interceptor[]{pageHelper});

		// xml目录配置
		ResourcePatternResolver resourcePatternResolver = new PathMatchingResourcePatternResolver();
		bean.setMapperLocations(resourcePatternResolver.getResources("classpath:mapper/*.xml"));

		//配置实体类属性与表字段的映射
		org.apache.ibatis.session.Configuration configuration = new org.apache.ibatis.session.Configuration();
		configuration.setMapUnderscoreToCamelCase(true);
		bean.setConfiguration(configuration);

		return bean.getObject();
	}

	@Bean
	public MapperScannerConfigurer mapperScannerConfigurer() {
		MapperScannerConfigurer mapperScannerConfigurer = new MapperScannerConfigurer();
		mapperScannerConfigurer.setBasePackage("cn.edu.seu.alumni_server.dao.mapper");
		mapperScannerConfigurer.setSqlSessionFactoryBeanName("sqlSessionFactory");
		Properties properties = new Properties();
		properties.setProperty("mappers", "tk.mybatis.mapper.common.Mapper");
		mapperScannerConfigurer.setProperties(properties);
		return mapperScannerConfigurer;
	}
}
