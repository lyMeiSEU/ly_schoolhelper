package cn.edu.seu.alumni_server.common;

import org.apache.http.entity.ContentType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.mock.web.MockMultipartFile;
import org.springframework.web.client.RestTemplate;
import org.springframework.web.multipart.MultipartFile;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Date;

public class Utils {

    static IdGenerator idGenerator;
    static String access_token = "";
    static long expireTime = 0l;

    static {
//        idGenerator = new SnowflakeIdGenerator(0, 0);
        idGenerator = new IdGenerator();
    }

    public static long generateId() {
        return idGenerator.nextId();
    }

    public static MultipartFile fileToMultipartFile(File file) throws IOException {
        FileInputStream inputStream = new FileInputStream(file);
        return new MockMultipartFile(file.getName(), file.getName(),
                ContentType.APPLICATION_OCTET_STREAM.toString(), inputStream);
    }

    public static Date addEightHours(Date date) {
        return new Date(date.getTime() + 8 * 60 * 60 * 1000);
    }

    public static Boolean deleteFileUnderProjectDir(String fileName) {
        // 然后应该删除项目目录下的本地文件
        File targetFile = new File(System.getProperty("user.dir") + File.separator + fileName);
        return targetFile.delete();
    }

    public String getAccessToken() {
        if (System.currentTimeMillis() > expireTime) {

        }
        return access_token;
    }
}
