package cn.edu.seu.alumni_server.controller.dto;

import cn.edu.seu.alumni_server.dao.entity.Message;
import lombok.Data;
import org.springframework.beans.BeanUtils;

@Data
public class MessageDTO {
    private Long messageId;

    private Integer type;
    private String typeDesc;

    private String title;
    private String content;

    /**
     * 0未读1已读
     * 默认未读
     */
    private Integer status = 0;

    private Long timestamp;

    private Long fromUser;
    private String fromUserName;
    private String avatar;

    private Long toUser;

    MessageDTO() {

    }

    public MessageDTO(Message message) {
        BeanUtils.copyProperties(message, this);
    }

    public Message toMessage() {
        Message message = new Message();
        BeanUtils.copyProperties(this, message);
        return message;
    }
}
