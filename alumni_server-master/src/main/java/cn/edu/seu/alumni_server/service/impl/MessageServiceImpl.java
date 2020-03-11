package cn.edu.seu.alumni_server.service.impl;

import cn.edu.seu.alumni_server.common.Utils;
import cn.edu.seu.alumni_server.dao.entity.Message;
import cn.edu.seu.alumni_server.dao.mapper.MessageMapper;
import cn.edu.seu.alumni_server.service.MessageService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class MessageServiceImpl implements MessageService {

    @Autowired
    MessageMapper messageMapper;

    @Override
    public void newMessage(Long fromUser, Long toUser, Integer type) {
        //消息通知
        Message message = new Message();
        message.setTimestamp(System.currentTimeMillis());
        message.setMessageId(Utils.generateId());

        message.setFromUser(fromUser);
        message.setToUser(toUser);
        message.setType(type);
        messageMapper.insertSelective(message);
//        messageMapper
    }

    @Override
    public void newMessage(Long fromUser, Long toUser, Integer type,String title, String content) {
        //消息通知
        Message message = new Message();
        message.setTimestamp(System.currentTimeMillis());
        message.setMessageId(Utils.generateId());

        message.setFromUser(fromUser);
        message.setToUser(toUser);
        message.setType(type);
        message.setContent(content);
        message.setTitle(title);
        messageMapper.insertSelective(message);
    }
}
