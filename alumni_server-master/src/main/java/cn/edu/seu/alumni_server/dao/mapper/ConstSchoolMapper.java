package cn.edu.seu.alumni_server.dao.mapper;

import cn.edu.seu.alumni_server.dao.entity.ConstSchool;
import org.springframework.stereotype.Repository;
import tk.mybatis.mapper.common.Mapper;

import java.util.List;

@Repository
public interface ConstSchoolMapper extends Mapper<ConstSchool> {
    List<ConstSchool> queryConstSchoolList(String schoolName);
}