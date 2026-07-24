#include "services/event_service.hpp"
#include "utils/date_validator.hpp"
#include <algorithm>
#include <numeric>
#include <vector>

EventService::EventService(IMessageSender &_client, DBManager &db_manager)
    : client(_client), db(db_manager) {}

void EventService::list(const std::vector<std::string> &) {

  if (!db.has_events()) {
    return;
  }
  auto events = db.get_events();
  for (const auto &event : events) {
    client.send_message("[" + event.id + "] " + event.value + " | " +
                        "Fecha: " + event.date);
  }
}

void EventService::remove(const std::vector<std::string> &args) {
  if (args.empty()) {
    return;
  }
  std::string number_str = args.front();

  bool is_a_number =
      std::all_of(number_str.begin(), number_str.end(),
                  [](unsigned char c) { return std::isdigit(c); });

  if (!is_a_number) {
    return;
  }

  auto success = db.delete_event(std::stoi(number_str));
  if (!success) {
    client.send_message("Evento no se pudo eliminar o no existe");
  }

  client.send_message("Evento eliminado :)");
}

void EventService::add(const std::vector<std::string> &args) {

  if (args.size() < 2) {
    client.send_message("Cantidad de argumentos incorrecto. uso /event add "
                        "<fecha> <hora> <contenido>");
    return;
  }

  std::string date = args[0];

  std::string hour = args[1];

  if (DateValidator::is_valid(date, hour)) {
    client.send_message(
        "Formato de fecha incorrecto. Ejemplo 2026-07-24 04:30");
    return;
  }

  std::string content =
      std::accumulate(args.begin() + 2, args.end(), std::string(),
                      [](const std::string &a, const std::string &b) {
                        return a.empty() ? b : a + " " + b;
                      });

  if (content.empty()) {
    client.send_message("No escribiste nada para guardar :(");
    return;
  }

  db.create_event(content, date + " " + hour);
}
