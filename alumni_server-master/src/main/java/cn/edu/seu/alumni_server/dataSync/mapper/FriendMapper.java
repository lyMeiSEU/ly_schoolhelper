package cn.edu.seu.alumni_server.dataSync.mapper;

import cn.edu.seu.alumni_server.dataSync.entity.Friend;
import org.springframework.stereotype.Repository;
import tk.mybatis.mapper.common.Mapper;

@Repository(value = "FriendMapperV1")
public interface FriendMapper extends Mapper<Friend> {
}