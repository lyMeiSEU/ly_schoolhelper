package cn.edu.seu.alumni_server.dao.mapper;

import cn.edu.seu.alumni_server.controller.dto.MessageDTO;
import cn.edu.seu.alumni_server.dao.entity.Message;
import org.springframework.stereotype.Repository;
import tk.mybatis.mapper.common.Mapper;

import java.util.List;

@Repository
public interface MessageMapper extends Mapper<Message> {
    List<MessageDTO> getMessagesByAccountId(Long accountId,Integer status);
}