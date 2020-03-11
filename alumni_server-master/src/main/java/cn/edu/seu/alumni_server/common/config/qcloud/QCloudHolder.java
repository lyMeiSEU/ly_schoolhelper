package cn.edu.seu.alumni_server.common.config.qcloud;

import com.qcloud.cos.COSClient;
import com.qcloud.cos.ClientConfig;
import com.qcloud.cos.auth.BasicCOSCredentials;
import com.qcloud.cos.auth.COSCredentials;
import com.qcloud.cos.region.Region;
import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
public class QCloudHolder {

	private QCloudCOSConfig qCloudCOSConfig;
	private Region region;
	private COSCredentials cred;
	private ClientConfig clientConfig;
	private String bucketName;
	private String baseUrl;

	public QCloudHolder(
		QCloudCOSConfig qCloudCOSConfig,
		String bucketName,
		String baseUrl
	) {
		this.init(qCloudCOSConfig, bucketName, baseUrl);
	}

	public void init(QCloudCOSConfig qCloudCOSConfig,
		String bucketName,
		String baseUrl
	) {
		this.qCloudCOSConfig = qCloudCOSConfig;
		this.region = new Region(qCloudCOSConfig.regionString);
		this.cred = new BasicCOSCredentials(qCloudCOSConfig.secretId, qCloudCOSConfig.secretKey);
		this.clientConfig = new ClientConfig(this.region);
		this.bucketName = bucketName;
		this.baseUrl = baseUrl;
	}

	public COSClient newCOSClient() {
		return new COSClient(this.cred, this.clientConfig);
	}

	public void closeCOSClient(COSClient cosClient) {
		cosClient.shutdown();
	}
}