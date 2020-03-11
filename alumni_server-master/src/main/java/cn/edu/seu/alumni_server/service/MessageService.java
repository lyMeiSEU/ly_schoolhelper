package cn.edu.seu.alumni_server.service;

public interface MessageService {

    void newMessage(Long fromUser, Long toUser, Integer type);

    void newMessage(Long fromUser, Long toUser, Integer type, String title,String content);
}
