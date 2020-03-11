package cn.edu.seu.alumni_server.dataSync.mapper;

import cn.edu.seu.alumni_server.dataSync.entity.Personal;
import org.springframework.stereotype.Repository;
import tk.mybatis.mapper.common.Mapper;

@Repository
public interface PersonalMapper extends Mapper<Personal> {
}