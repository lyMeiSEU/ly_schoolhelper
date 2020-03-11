package cn.edu.seu.alumni_server.dao.mapper;

import cn.edu.seu.alumni_server.controller.dto.FriendDTO;
import cn.edu.seu.alumni_server.dao.entity.Friend;
import org.springframework.stereotype.Repository;
import tk.mybatis.mapper.common.Mapper;

import java.util.List;

@Repository
public interface FriendMapper extends Mapper<Friend> {
    List<FriendDTO> getFriends(Long accountId);

    Friend getRelationShip(long myAccountId, long accountId);

    int insertOnDuplicateKeyUpdate(Friend f);
}