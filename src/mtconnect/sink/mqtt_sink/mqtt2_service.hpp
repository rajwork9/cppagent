  //
  // Copyright Copyright 2009-2023, AMT – The Association For Manufacturing Technology (“AMT”)
  // All rights reserved.
  //
  //    Licensed under the Apache License, Version 2.0 (the "License");
  //    you may not use this file except in compliance with the License.
  //    You may obtain a copy of the License at
  //
  //       http://www.apache.org/licenses/LICENSE-2.0
  //
  //    Unless required by applicable law or agreed to in writing, software
  //    distributed under the License is distributed on an "AS IS" BASIS,
  //    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  //    See the License for the specific language governing permissions and
  //    limitations under the License.
  //

  #pragma once

  #include <nlohmann/json.hpp>

  #include "boost/asio/io_context.hpp"
  #include <boost/dll/alias.hpp>

  #include "mtconnect/buffer/checkpoint.hpp"
  #include "mtconnect/config.hpp"
  #include "mtconnect/configuration/agent_config.hpp"
  #include "mtconnect/entity/json_printer.hpp"
  #include "mtconnect/mqtt/mqtt_client.hpp"
  #include "mtconnect/observation/observation.hpp"
  #include "mtconnect/printer/printer.hpp"
#include "mtconnect/printer//json_printer.hpp"
  #include "mtconnect/printer/xml_printer_helper.hpp"
  #include "mtconnect/sink/sink.hpp"
  #include "mtconnect/utilities.hpp"

  using namespace std;
  using namespace mtconnect;
  using namespace mtconnect::entity;
  using namespace mtconnect::mqtt_client;

  using json = nlohmann::json;

  namespace mtconnect {
    class XmlPrinter;

    namespace sink {

      /// @brief MTConnect Mqtt implemention namespace

      namespace mqtt_sink {

        struct AsyncCurrentServiceResponse;

        class AGENT_LIB_API Mqtt2Service : public sink::Sink
        {
          // dynamic loading of sink

        public:
          /// @brief Create a Mqtt Service sink
          /// @param context the boost asio io_context
          /// @param contract the Sink Contract from the agent
          /// @param options configuration options
          /// @param config additional configuration options if specified directly as a sink
          Mqtt2Service(boost::asio::io_context &context, sink::SinkContractPtr &&contract,
                      const ConfigOptions &options, const boost::property_tree::ptree &config);

          ~Mqtt2Service() = default;

          // Sink Methods
          /// @brief Start the Mqtt service
          void start() override;

          /// @brief Shutdown the Mqtt service
          void stop() override;

          /// @brief Receive an observation
          /// @param observation shared pointer to the observation
          /// @return `true` if the publishing was successful
          bool publish(observation::ObservationPtr &observation) override;

          /// @brief Receive an asset
          /// @param asset shared point to the asset
          /// @return `true` if successful
          bool publish(asset::AssetPtr asset) override;

          /// @brief Receive a device
          /// @param device shared pointer to the device
          /// @return `true` if successful
          bool publish(device_model::DevicePtr device) override;

          void createProbeRoutings();

          void createCurrentRoutings();

          void streamNextCurrentService(std::shared_ptr<AsyncCurrentServiceResponse> asyncResponse,
              boost::system::error_code ec);
          
          void streamNextCurrentService(const int interval,
                                        const std::optional<std::string> &device);

          bool probeRequest(const std::optional<std::string> &device);

          DevicePtr checkDevice(const std::string &uuid) const;           

          /// @brief Register the Sink factory to create this sink
          /// @param factory
          static void registerFactory(SinkFactory &factory);

          /// @brief gets a Mqtt Client
          /// @return MqttClient
          std::shared_ptr<MqttClient> getClient();

          /// @brief Mqtt Client is Connected or not
          /// @return `true` when the client was connected
          bool isConnected() { return m_client && m_client->isConnected(); }

        protected:

          std::string m_devicePrefix;
          
          std::string m_assetPrefix;
          
          std::string m_observationPrefix;
          
          uint64_t m_instanceId;
          
          boost::asio::io_context &m_context;

          boost::asio::io_context::strand m_strand;

          ConfigOptions m_options;
          
          std::unique_ptr<JsonEntityPrinter> m_jsonPrinter;
          
          std::unique_ptr<printer::JsonPrinter> m_printer;
          
          std::shared_ptr<MqttClient> m_client;
          
          boost::asio::steady_timer m_timer;

        };
      }  // namespace mqtt_sink
    }    // namespace sink
  }  // namespace mtconnect
