package cn.edu.seu.alumni_server.dao.mapper;

import cn.edu.seu.alumni_server.dao.entity.Like;
import org.springframework.stereotype.Repository;
import tk.mybatis.mapper.common.Mapper;

import java.util.List;

@Repository
public interface LikeMapper extends Mapper<Like> {

    List<Like> queryLikeList(Like like);

    Long insertLike(Like like);

    int updateLike(Like like);
}
