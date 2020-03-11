package cn.edu.seu.alumni_server.dataSync.mapper;

import cn.edu.seu.alumni_server.dataSync.entity.Education;
import org.springframework.stereotype.Repository;
import tk.mybatis.mapper.common.Mapper;

@Repository(value = "EducationMapperV1")
public interface EducationMapper extends Mapper<Education> {
}