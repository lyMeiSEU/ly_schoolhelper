package cn.edu.seu.alumni_server.dataSync.mapper;

import cn.edu.seu.alumni_server.dataSync.entity.Work;
import org.springframework.stereotype.Repository;
import tk.mybatis.mapper.common.Mapper;

@Repository
public interface WorkMapper extends Mapper<Work> {
}